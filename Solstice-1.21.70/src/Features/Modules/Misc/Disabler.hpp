#pragma once
//
// Created by vastrakai on 8/9/2024.
//

class Disabler : public ModuleBase<Disabler> {
public:
    enum class Mode {
        Flareon,
        Sentinel,
        Lifeboat,
#ifdef __PRIVATE_BUILD__ // we're not allowed to release SentinelNew, do NOT include this in the final build
        SentinelNew,
#endif
        Custom,
        BDSPrediction,
        Test
    };
    enum class DisablerType {
        PingSpoof,
        PingHolder,
#ifdef __PRIVATE_BUILD__
        MoveFix,
        MoveFixV2
#endif
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode to use for the disabler.", Mode::Flareon, "Flareon", "Sentinel", "Lifeboat",
#ifdef __PRIVATE_BUILD__
        "SentinelNew",
#endif
        "Custom", "BDS Prediction", "Test");
    EnumSettingT<DisablerType> mDisablerType = EnumSettingT<DisablerType>("Disabler Type", "The type of disabler to use.", DisablerType::PingSpoof, "Ping Spoof", "Ping Holder"
#ifdef __PRIVATE_BUILD__
        ,"Move Fix", "Move Fix V2"
#endif
        );
#ifdef __PRIVATE_BUILD__
    NumberSetting mQueuedPackets = NumberSetting("Queued Packets", "The amount of packets to queue", 120, 0, 300, 1);
    BoolSetting mReverseQueue = BoolSetting("Reverse Queue", "Whether or not to reverse the queue", false);
    NumberSetting mDropChance = NumberSetting("Drop Chance", "The chance to drop the packet", 0, 0, 100, 1);
#endif
    BoolSetting mRandomizeDelay = BoolSetting("Randomize Delay", "Whether or not to randomize the delay", true);
    NumberSetting mDelay = NumberSetting("Delay", "The delay to use for the disabler", 10000, 0, 100000, 1);
    NumberSetting mMinDelay = NumberSetting("Min Delay", "The minimum delay to randomize", 6000, 0, 100000, 1);
    NumberSetting mMaxDelay = NumberSetting("Max Delay", "The maximum delay to randomize", 10000, 0, 100000, 1);
    BoolSetting mInteract = BoolSetting("Interact", "Whether or not to send interact packet before attack", false);
    BoolSetting mCancel = BoolSetting("Cancel", "Whether or not to cancel networkstacklatency packet", false);
    BoolSetting mGlide = BoolSetting("Glide", "Whether or not to send start gliding packet", false);
    BoolSetting mOnGroundSpoof = BoolSetting("On Ground Spoof", "Whether or not to spoof on ground", false);
    BoolSetting mInputSpoof = BoolSetting("Input Spoof", "Whether or not to spoof input mode", false);
    BoolSetting mClickPosFix = BoolSetting("Click Pos Fix", "Whether or not to fix click pos", false);

    Disabler() : ModuleBase<Disabler>("Disabler", "Attempts to disable Anti-Cheat checks by exploiting them.", ModuleCategory::Misc, 0, false){
        addSetting(&mMode);
        addSetting(&mDisablerType);
#ifdef __DEBUG__
        addSetting(&mQueuedPackets);
        addSetting(&mReverseQueue);
        addSetting(&mDropChance);
#endif
        addSetting(&mRandomizeDelay);
        addSetting(&mDelay);
        addSetting(&mMinDelay);
        addSetting(&mMaxDelay);
        addSettings(&mInteract, &mCancel, &mGlide, &mOnGroundSpoof, &mInputSpoof, &mClickPosFix);

        VISIBILITY_CONDITION(mDisablerType, mMode.mValue == Mode::Flareon);
#ifdef __DEBUG__
        VISIBILITY_CONDITION(mQueuedPackets, mMode.mValue == Mode::SentinelNew);
        VISIBILITY_CONDITION(mReverseQueue, mMode.mValue == Mode::SentinelNew);
        VISIBILITY_CONDITION(mDropChance, mMode.mValue == Mode::SentinelNew);
#endif
        VISIBILITY_CONDITION(mRandomizeDelay, mMode.mValue == Mode::Flareon && mDisablerType.mValue == DisablerType::PingSpoof);

        VISIBILITY_CONDITION(mDelay, mMode.mValue == Mode::Flareon && mDisablerType.mValue == DisablerType::PingSpoof && !mRandomizeDelay.mValue);
        VISIBILITY_CONDITION(mMinDelay, mMode.mValue == Mode::Flareon && mDisablerType.mValue == DisablerType::PingSpoof && mRandomizeDelay.mValue);
        VISIBILITY_CONDITION(mMaxDelay, mMode.mValue == Mode::Flareon && mDisablerType.mValue == DisablerType::PingSpoof && mRandomizeDelay.mValue);

        VISIBILITY_CONDITION(mInteract, mMode.mValue == Mode::Custom);
        VISIBILITY_CONDITION(mCancel, mMode.mValue == Mode::Custom);
        VISIBILITY_CONDITION(mGlide, mMode.mValue == Mode::Custom);
        VISIBILITY_CONDITION(mOnGroundSpoof, mMode.mValue == Mode::Custom);
        VISIBILITY_CONDITION(mInputSpoof, mMode.mValue == Mode::Custom);
        VISIBILITY_CONDITION(mClickPosFix, mMode.mValue == Mode::Custom);

        mNames = {
            {Lowercase, "disabler"},
            {LowercaseSpaced, "disabler"},
            {Normal, "Disabler"},
            {NormalSpaced, "Disabler"}
        };
    }

    uint64_t mClientTicks = 0;
    bool mShouldUpdateClientTicks = false;
    glm::vec3 mLastPosition = { 0, 0, 0 };
    Actor* mFirstAttackedActor = nullptr;

    std::map<int64_t, uint64_t> mPacketQueue;
    void sortQueueByTime()
    {
        // Sort by lowest .first value
        mPacketQueue = std::map<int64_t, uint64_t>(mPacketQueue.begin(), mPacketQueue.end());
    }

    void onEnable() override;
    void onDisable() override;
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    void onRunUpdateCycleEvent(class RunUpdateCycleEvent& event);
    int64_t getDelay() const;
    void onPingUpdateEvent(class PingUpdateEvent& event);
    void onSendImmediateEvent(class SendImmediateEvent& event);
};