// 7/28/2024
#pragma once
#include <Features/Modules/Module.hpp>

class OreMiner : public ModuleBase<OreMiner> {
public:
    enum class UncoverMode {
        None,
        PathFind,
#ifdef __PRIVATE_BUILD__
        UnderGround,
#endif
    };
    enum class CalcMode {
        Normal,
        Dynamic
    };
    enum class OrePriority {
        High,
        Medium,
        Low
    };
    enum class RotationMode {
        Normal,
    };
    enum class SwingMode {
        Normal,
        Silent
    };
    EnumSettingT<UncoverMode> mUncoverMode = EnumSettingT<UncoverMode>("Uncover Mode", "The mode for uncover", UncoverMode::None, "None", "Path Find"
#ifdef __PRIVATE_BUILD__
    ,"Under Ground"
#endif
    );
    EnumSettingT<CalcMode> mCalcMode = EnumSettingT<CalcMode>("Calc Mode", "The calculation mode destroy speed", CalcMode::Normal, "Normal", "Dynamic");
    BoolSetting mOnGroundOnly = BoolSetting("OnGround Only", "use dynamic destroy speed only on ground", false);
    NumberSetting mRange = NumberSetting("Range", "The max range for destroying blocks", 5, 0, 10, 0.01);
    NumberSetting mUncoverRange = NumberSetting("Uncover Range", "The max range for uncovering blocks", 3, 1, 8, 1);
    NumberSetting mDestroySpeed = NumberSetting("Destroy Speed", "The destroy speed for Ore miner", 1, 0.01, 1, 0.01);
    BoolSetting mEmerald = BoolSetting("Emerald", "Destroy emerald ore", false);
    EnumSettingT<OrePriority> mEmeraldPriority = EnumSettingT<OrePriority>("Emerald Priority", "The priority for emerald ore", OrePriority::Medium, "High", "Medium", "Low");
    BoolSetting mDiamond = BoolSetting("Diamond", "Destroy diamond ore", false);
    EnumSettingT<OrePriority> mDiamondPriority = EnumSettingT<OrePriority>("Diamond Priority", "The priority for diamond ore", OrePriority::Medium, "High", "Medium", "Low");
    BoolSetting mGold = BoolSetting("Gold", "Destroy gold ore", false);
    EnumSettingT<OrePriority> mGoldPriority = EnumSettingT<OrePriority>("Gold Priority", "The priority for gold ore", OrePriority::Medium, "High", "Medium", "Low");
    BoolSetting mIron = BoolSetting("Iron", "Destroy iron ore", false);
    EnumSettingT<OrePriority> mIronPriority = EnumSettingT<OrePriority>("Iron Priority", "The priority for iron ore", OrePriority::Medium, "High", "Medium", "Low");
    BoolSetting mCoal = BoolSetting("Coal", "Destroy coal ore", false);
    EnumSettingT<OrePriority> mCoalPriority = EnumSettingT<OrePriority>("Coal Priority", "The priority for coal ore", OrePriority::Medium, "High", "Medium", "Low");
    BoolSetting mRedstone = BoolSetting("Redstone", "Destroy redstone ore", false);
    EnumSettingT<OrePriority> mRedstonePriority = EnumSettingT<OrePriority>("Redstone Priority", "The priority for redstone ore", OrePriority::Medium, "High", "Medium", "Low");
    BoolSetting mLapis = BoolSetting("Lapis", "Destroy lapis ore", false);
    EnumSettingT<OrePriority> mLapisPriority = EnumSettingT<OrePriority>("Lapis Priority", "The priority for lapis ore", OrePriority::Medium, "High", "Medium", "Low");
    BoolSetting mSwing = BoolSetting("Swing", "Swings when destroying blocks", false);
    BoolSetting mHotbarOnly = BoolSetting("Hotbar Only", "Only switch to tools in the hotbar", false);
    BoolSetting mInfiniteDurability = BoolSetting("Infinite Durability", "Infinite durability for tools (may cause issues!)", false);
    BoolSetting mFlareonV2 = BoolSetting("FlareonV2", "Add settings to bypass flareon v2", false);
    EnumSettingT<RotationMode> mRotationMode = EnumSettingT<RotationMode>("Rotation Mode", "The rotation mode", RotationMode::Normal, "Normal");
    BoolSetting mHeadYaw = BoolSetting("Head Yaw", "Avoids movement check", false);
    EnumSettingT<SwingMode> mSwingMode = EnumSettingT<SwingMode>("Swing Mode", "The swing mode", SwingMode::Normal, "Normal", "Silent");
    BoolSetting mOnGround = BoolSetting("OnGround", "Don't break block while you're off ground", false);
    BoolSetting mFirstRotation = BoolSetting("First Rotation", "Rotate when queue block", true);
    BoolSetting mRenderBlock = BoolSetting("Render Block", "Renders the block you are currently breaking", true);

    OreMiner() : ModuleBase("OreMiner", "Automatically breaks ores", ModuleCategory::Player, 0, false) {
        addSetting(&mUncoverMode);
        addSetting(&mCalcMode);
        addSetting(&mOnGroundOnly);
        addSetting(&mRange);
        addSetting(&mUncoverRange);
        addSetting(&mDestroySpeed);
        addSetting(&mEmerald);
        addSetting(&mEmeraldPriority);
        addSetting(&mDiamond);
        addSetting(&mDiamondPriority);
        addSetting(&mGold);
        addSetting(&mGoldPriority);
        addSetting(&mIron);
        addSetting(&mIronPriority);
        addSetting(&mCoal);
        addSetting(&mCoalPriority);
        addSetting(&mRedstone);
        addSetting(&mRedstonePriority);
        addSetting(&mLapis);
        addSetting(&mLapisPriority);
        addSetting(&mSwing);
        addSetting(&mHotbarOnly);
        addSetting(&mInfiniteDurability);
        addSettings(&mFlareonV2, &mRotationMode, &mHeadYaw, &mSwingMode, &mOnGround, &mFirstRotation);
        addSetting(&mRenderBlock);

        VISIBILITY_CONDITION(mOnGroundOnly, mCalcMode.mValue == CalcMode::Dynamic);

        VISIBILITY_CONDITION(mEmeraldPriority, mEmerald.mValue);
        VISIBILITY_CONDITION(mDiamondPriority, mDiamond.mValue);
        VISIBILITY_CONDITION(mGoldPriority, mGold.mValue);
        VISIBILITY_CONDITION(mIronPriority, mIron.mValue);
        VISIBILITY_CONDITION(mCoalPriority, mCoal.mValue);
        VISIBILITY_CONDITION(mRedstonePriority, mRedstone.mValue);
        VISIBILITY_CONDITION(mLapisPriority, mLapis.mValue);

        VISIBILITY_CONDITION(mRotationMode, mFlareonV2.mValue);
        VISIBILITY_CONDITION(mHeadYaw, mFlareonV2.mValue);
        VISIBILITY_CONDITION(mSwingMode, mFlareonV2.mValue);
        VISIBILITY_CONDITION(mOnGround, mFlareonV2.mValue);
        VISIBILITY_CONDITION(mFirstRotation, mFlareonV2.mValue);
        
        mNames = {
            {Lowercase, "oreminer"},
            {LowercaseSpaced, "ore miner"},
            {Normal, "OreMiner"},
            {NormalSpaced, "Ore Miner"}
        };
    }

    struct PathFindResult {
        glm::ivec3 blockPos;
        bool foundPath;
    };

    static inline glm::ivec3 mCurrentBlockPos = { INT_MAX, INT_MAX, INT_MAX };
    glm::ivec3 mTargettingBlockPos = { INT_MAX, INT_MAX, INT_MAX };
    int mCurrentBlockFace = -1;
    float mBreakingProgress = 0.f;
    float mCurrentDestroySpeed = 1.f;
    static inline bool mIsMiningBlock = false;
    bool mWasMiningBlock = false;
    bool mIsUncovering = false;
    bool mShouldRotate = false;
    bool mShouldSpoofSlot = false;
    bool mShouldSetbackSlot = false;
    int mPreviousSlot = -1;
    int mToolSlot = -1;
    bool mOffGround = false;

    uint64_t mLastBlockPlace = 0;
    int mLastPlacedBlockSlot = 0;

    std::vector<glm::ivec3> mOffsetList = {
        glm::ivec3(0, -1, 0),
        glm::ivec3(0, 1, 0),
        glm::ivec3(0, 0, -1),
        glm::ivec3(0, 0, 1),
        glm::ivec3(-1, 0, 0),
        glm::ivec3(1, 0, 0),
    };

    std::vector<int> mEmeraldIds = {
        129,
        662,
    };

    std::vector<int> mDiamondIds = {
        56,
        660,
    };

    std::vector<int> mGoldIds = {
        14,
        657,
    };

    std::vector<int> mIronIds = {
        15,
        656,
    };

    std::vector<int> mCoalIds = {
        16,
        661,
    };

    std::vector<int> mRedstoneIds = {
        73,
        74,
        658,
        659,
    };

    std::vector<int> mLapisIds = {
        21,
        655,
    };

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onRenderEvent(class RenderEvent& event);
    void renderBlock();
    void onPacketOutEvent(class PacketOutEvent& event);
    void reset();
    void queueBlock(glm::ivec3 blockPos);
    bool isValidBlock(glm::ivec3 blockPos, bool oreOnly, bool exposedOnly, bool usePriority = false, OrePriority priority = OrePriority::Medium);
    PathFindResult getBestPathToBlock(glm::ivec3 blockPos);
    bool isOre(std::vector<int> Ids, int id);

    std::string getSettingDisplay() override {
        return mUncoverMode.mValues[mUncoverMode.as<int>()];
    }
};