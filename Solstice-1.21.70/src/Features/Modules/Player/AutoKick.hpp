#pragma once
//
// 8/22/2024.
//

#include <Features/Modules/Module.hpp>

class AutoKick : public ModuleBase<AutoKick> {
public:
    enum class Mode {
        Push,
        Replace
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode for auto kick", Mode::Push, "Push", "Replace");
    NumberSetting mRange = NumberSetting("Range", "The range at which to push entities", 5, 3, 8, 0.1);
    NumberSetting mDelay = NumberSetting("Delay", "The delay in ms to place block", 1000, 100, 5000, 10);
    BoolSetting mHotbarOnly = BoolSetting("Hotbar Only", "Only switch to blocks in the hotbar", true);
    BoolSetting mAllowDiagonal = BoolSetting("Allow Diagonal", "Allow diagonal prediction", true);
    BoolSetting mOnGroundOnly = BoolSetting("OnGround Only", "Place block while target is onground", true);
    NumberSetting mMaxOnGroundTicks = NumberSetting("OnGround Ticks", "The max onground ticks to place block", 10, 0, 100, 5);
    NumberSetting mOpponentPing = NumberSetting("Opponent Ping", "Specify opponent ping time", 90, 0, 300, 10);
    BoolSetting mSpamPlace = BoolSetting("Spam Place", "Reset delay if block ghosted", false);
    NumberSetting mDestroySpeed = NumberSetting("Destroy Speed", "The destroy speed for fast mine", 1, 0.01, 1, 0.01);
    BoolSetting mInfiniteDurability = BoolSetting("Infinite Durability", "Infinite durability for tools (may cause issues!)", false);
    BoolSetting mDebug = BoolSetting("Debug", "Send debug message in chat", false);
    AutoKick() : ModuleBase("AutoKick", "Attempts to kick players by placing blocks in front of them", ModuleCategory::Player, 0, false)
    {
        addSetting(&mMode);
        addSetting(&mRange);
        addSetting(&mDelay);
        addSetting(&mHotbarOnly);
        addSetting(&mAllowDiagonal);
        addSetting(&mOnGroundOnly);
        addSetting(&mMaxOnGroundTicks);
        addSetting(&mOpponentPing);
        addSetting(&mSpamPlace);
        addSetting(&mDestroySpeed);
        addSetting(&mInfiniteDurability);
        addSetting(&mDebug);

        VISIBILITY_CONDITION(mMaxOnGroundTicks, mOnGroundOnly.mValue);

        mNames = {
              {Lowercase, "autokick"},
                {LowercaseSpaced, "auto kick"},
                {Normal, "AutoKick"},
                {NormalSpaced, "Auto Kick"}
        };
    };

    int mPreviousSlot = 0;

    // Push
    bool mSelectedSlot = false;
    bool mShouldRotate = false;

    glm::vec3 mLastTargetPos = { INT_MAX, INT_MAX, INT_MAX };
    bool mUsePrediction = false;
    int mOnGroundTicks = 0;

    uint64_t mLastBlockPlace = 0;
    glm::ivec3 mCurrentPlacePos = { INT_MAX, INT_MAX, INT_MAX };
    uint64_t mLastBlockUpdated = 0;
    glm::ivec3 mLastServerBlockPos = { INT_MAX, INT_MAX, INT_MAX };
    bool mFlagged = false;

    uint64_t mPing = 100;
    uint64_t mEventDelay = 0;

    std::vector<glm::ivec3> mRecentlyUpdatedBlockPositions;

    std::map<std::string, int> mFlagCounter;

    // Replace
    std::vector<glm::ivec3> mMiningBlocks;
    glm::ivec3 mCurrentBlockPos = { INT_MAX, INT_MAX, INT_MAX };
    int mCurrentBlockFace = 0;
    int mToolSlot = 0;
    bool mIsMiningBlock = false;
    bool mShouldRotateToBlock = false;
    bool mShouldSpoofSlot = false;
    bool mShouldSetbackSlot = false;
    bool mWasPlacingBlock = false;
    float mBreakingProgress = 0.f;

    void onEnable();
    void onDisable() override;
    void reset();
    bool isValidBlock(glm::ivec3 blockPos);
    void queueBlock(glm::ivec3 blockPos);
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onSendImmediateEvent(class SendImmediateEvent& event);
    void onPingUpdateEvent(class PingUpdateEvent& event);
};