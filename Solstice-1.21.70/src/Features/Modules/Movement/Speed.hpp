#pragma once
//
// Created by vastrakai on 7/10/2024.
//
#include <Features/FeatureManager.hpp>
#include <Features/Modules/Module.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/Packets/MobEffectPacket.hpp>

enum class JumpType {
    Vanilla,
    Velocity,
    None
};


enum class FastfallMode {
    None,
    Predict,
    SetVel
};


struct FrictionPreset
{
    float speed = 0.5;
    bool strafe = true;
    bool useStrafeSpeed = true;
    float strafeSpeed = 0.5;
    float friction = 0.975;
    bool timerBoost = false;
    float timerSpeed = 20;
    FastfallMode fastFall = FastfallMode::None;
    int fallTicks = 5;
    float fallSpeed = 1.00;
    bool fastFall2 = false;
    int fallTicks2 = 5;
    float fallSpeed2 = 1.00;
    JumpType jumpType = JumpType::Vanilla;
    float jumpHeight = 0.42f;

    FrictionPreset() = default;
    FrictionPreset(float speed, bool strafe, bool useStrafeSpeed, float strafeSpeed, float friction, bool timerBoost, float timerSpeed, FastfallMode fastFall, int fallTicks, float fallSpeed, bool fastFall2, int fallTicks2, float fallSpeed2, JumpType jumpType, float jumpHeight)
        : speed(speed), strafe(strafe), useStrafeSpeed(useStrafeSpeed), strafeSpeed(strafeSpeed), friction(friction), timerBoost(timerBoost), timerSpeed(timerSpeed), fastFall(fastFall), fallTicks(fallTicks), fallSpeed(fallSpeed), fastFall2(fastFall2), fallTicks2(fallTicks2), fallSpeed2(fallSpeed2), jumpType(jumpType), jumpHeight(jumpHeight)
    {}
};

class Speed : public ModuleBase<Speed> {
public:
    enum class Mode {
        Friction,
        Legit
    };

    enum class BypassMode {
        Always,
        StrafeOnly
    };

    EnumSettingT<Mode> mMode = EnumSettingT("Mode", "The mode of speed", Mode::Friction, "Friction", "Legit");
    BoolSetting mSwiftness = BoolSetting("Swiftness", "Whether or not to apply swiftness when space is pressed (will not be applied when scaffold is enabled)", false);
    BoolSetting mSwiftnessHotbar = BoolSetting("Swiftness Hotbar", "Only uses swiftness from hotbar", false);
    BoolSetting mHoldSpace = BoolSetting("Hold Space", "Only applies swiftness effect while holding space", false);
    NumberSetting mSwiftnessSpeed = NumberSetting("Swiftness Speed", "The speed to apply when swiftness is active", 0.55, 0, 1, 0.01);
    NumberSetting mSwiftnessFriction = NumberSetting("Swiftness Friction", "The friction to apply when swiftness is active", 0.975, 0, 1, 0.01);
    NumberSetting mSwiftnessTimer = NumberSetting("Swiftness Timer", "Whether or not to boost timer speed when swiftness is active", 35, 0, 40, 0.1);
#ifdef __PRIVATE_BUILD__
    BoolSetting mSwiftnessJump = BoolSetting("Swiftness Jump", "Whether or not to jump when swiftness is active", false);
#endif

    BoolSetting mDamageBoost = BoolSetting("Damage Boost", "Whether or not to boost speed when taking damage", false);
    NumberSetting mDamageBoostSpeed = NumberSetting("Damage Boost Speed", "The speed to boost when taking damage", 3, 1, 10, 0.01);
    NumberSetting mDamageBoostSlowdown = NumberSetting("Damage Boost Slowdown", "The friction to apply when taking damage", 0.15, 0, 1, 0.01);
    BoolSetting mDamageTimer = BoolSetting("Damage Timer", "Whether or not to boost timer speed when taking damage", false);
    NumberSetting mDamageTimerSpeed = NumberSetting("Damage Timer Speed", "The speed to boost timer by when taking damage", 20, 0, 40, 0.1);
    BoolSetting mDontBoosStrafeSpeed = BoolSetting("Ignore Strafe", "Dont increase strafe speed", true);

    NumberSetting mSpeed = NumberSetting("Speed", "The speed to move at", 0.5, 0, 10, 0.01);
    BoolSetting mRandomizeSpeed = BoolSetting("Randomize Speed", "Whether or not to randomize speed", false);
    NumberSetting mMinSpeed = NumberSetting("Min Speed", "The min speed to move at", 0.5, 0, 10, 0.01);
    NumberSetting mMaxSpeed = NumberSetting("Max Speed", "The max speed to move at", 1, 0, 10, 0.01);
    BoolSetting mStrafe = BoolSetting("Strafe Only", "Whether or not to allow strafing", true);
    BoolSetting mAvoidCheck = BoolSetting("Avoid Check", "Avoid strafe check in The Hive", false);
    EnumSettingT<BypassMode> mBypassMode = EnumSettingT("Bypass Mode", "The mode of avoid check", BypassMode::Always, "Always", "Strafe Only");
    NumberSetting mAvoidCheckDelay = NumberSetting("Delay", "The delay for avoid check", 400, 0, 3000, 50);
    BoolSetting mDebug = BoolSetting("Debug", "Send message in chat when applied strafe bypass", false);
    BoolSetting mTest = BoolSetting("Test", "test", true);
    BoolSetting mUseStrafeSpeed = BoolSetting("Custom Strafe Speed", "Whether or not to apply custom speed when strafing", true);
    NumberSetting mStrafeSpeed = NumberSetting("Strafe Speed", "The speed to strafe at", 0.5, 0, 10, 0.01);
    NumberSetting mFriction = NumberSetting("Friction", "The friction to apply", 0.975, 0, 1, 0.01);
    BoolSetting mTimerBoost = BoolSetting("Timer Boost", "Whether or not to boost timer speed", false);
    NumberSetting mTimerSpeed = NumberSetting("Timer Speed", "The speed to boost timer by", 20, 0, 40, 0.1);

    EnumSettingT<FastfallMode> mFastFall = EnumSettingT("Fast Fall", "The mode of fast fall", FastfallMode::None, "None", "Predict", "Set Vel");
    NumberSetting mFallTicks = NumberSetting("Fall Ticks", "The tick to apply down motion at", 5, 0, 20, 1);
    NumberSetting mFallSpeed = NumberSetting("Fall Speed", "The speed to fall down at", 1.00, 0, 10, 0.01);
    BoolSetting mFastFall2 = BoolSetting("Fast Fall 2", "Whether or not to fast fall again", false);
    NumberSetting mFallTicks2 = NumberSetting("Fall Ticks 2", "The tick to apply down motion at", 5, 0, 20, 1);
    NumberSetting mFallSpeed2 = NumberSetting("Fall Speed 2", "The speed to fall down at", 1.00, 0, 10, 0.01);

    EnumSettingT<JumpType> mJumpType = EnumSettingT("Jump Type", "The type of jump to use", JumpType::Vanilla, "Vanilla", "Velocity", "None");
    NumberSetting mJumpHeight = NumberSetting("Jump Height", "The height to jump at", 0.42f, 0, 1, 0.01);
    BoolSetting mApplyNetskip = BoolSetting("Apply Netskip", "Apply Netskip", false);

    BoolSetting mExtraHeight = BoolSetting("Extra Height", "Extra Height", false);
    NumberSetting mClipHeight = NumberSetting("Clip Height", "The height of clip", 1.00, 0, 2, 0.1);

    Speed() : ModuleBase("Speed", "Lets you move faster", ModuleCategory::Movement, 0, false) {
        addSettings(
            &mMode,
            &mSwiftness,
            &mSwiftnessHotbar,
            &mSwiftnessSpeed,
            &mSwiftnessFriction,
            &mSwiftnessTimer,
#ifdef __PRIVATE_BUILD__
            &mSwiftnessJump,
#endif
            &mHoldSpace,
            &mDamageBoost,
            &mDamageBoostSpeed,
            &mDamageBoostSlowdown,
            &mDamageTimer,
            &mDamageTimerSpeed,
            &mDontBoosStrafeSpeed,
            &mSpeed,
            &mRandomizeSpeed,
            &mMinSpeed,
            &mMaxSpeed,
            &mStrafe,
#ifdef __PRIVATE_BUILD__
            &mAvoidCheck,
            &mBypassMode,
            &mAvoidCheckDelay,
            &mDebug,
#endif
            &mTest,
            &mUseStrafeSpeed,
            &mStrafeSpeed,
            &mFriction,
            &mTimerBoost,
            &mTimerSpeed,
            &mFastFall,
            &mFallTicks,
            &mFallSpeed,
            &mFastFall2,
            &mFallTicks2,
            &mFallSpeed2,
            &mJumpType,
            &mJumpHeight,
            &mApplyNetskip
        );
#ifdef __PRIVATE_BUILD__
        addSettings(&mExtraHeight, &mClipHeight);
        VISIBILITY_CONDITION(mExtraHeight, mJumpType.mValue != JumpType::None);
        VISIBILITY_CONDITION(mClipHeight, mJumpType.mValue != JumpType::None && mExtraHeight.mValue);

        VISIBILITY_CONDITION(mAvoidCheck, mStrafe.mValue);
        VISIBILITY_CONDITION(mBypassMode, mStrafe.mValue && mAvoidCheck.mValue);
        VISIBILITY_CONDITION(mAvoidCheckDelay, mStrafe.mValue && mAvoidCheck.mValue);
        VISIBILITY_CONDITION(mDebug, mStrafe.mValue && mAvoidCheck.mValue);
#endif

        VISIBILITY_CONDITION(mSpeed, !mRandomizeSpeed.mValue)
        VISIBILITY_CONDITION(mMinSpeed, mRandomizeSpeed.mValue)
        VISIBILITY_CONDITION(mMaxSpeed, mRandomizeSpeed.mValue)

        VISIBILITY_CONDITION(mUseStrafeSpeed, mStrafe.mValue);
        VISIBILITY_CONDITION(mStrafeSpeed, mStrafe.mValue);

        VISIBILITY_CONDITION(mSwiftnessHotbar, mSwiftness.mValue);
        VISIBILITY_CONDITION(mSwiftnessSpeed, mSwiftness.mValue);
        VISIBILITY_CONDITION(mSwiftnessFriction, mSwiftness.mValue);
        VISIBILITY_CONDITION(mSwiftnessTimer, mSwiftness.mValue);
        VISIBILITY_CONDITION(mHoldSpace, mSwiftness.mValue);

        VISIBILITY_CONDITION(mDamageBoostSpeed, mDamageBoost.mValue);
        VISIBILITY_CONDITION(mDamageBoostSlowdown, mDamageBoost.mValue);
        VISIBILITY_CONDITION(mDamageTimer, mDamageBoost.mValue);
        VISIBILITY_CONDITION(mDamageTimerSpeed, mDamageBoost.mValue && mDamageTimer.mValue);
        VISIBILITY_CONDITION(mDontBoosStrafeSpeed, mDamageBoost.mValue);

        VISIBILITY_CONDITION(mStrafe, mMode.mValue != Mode::Legit);
        VISIBILITY_CONDITION(mTest, mMode.mValue != Mode::Legit);
        VISIBILITY_CONDITION(mUseStrafeSpeed, mMode.mValue != Mode::Legit);
        VISIBILITY_CONDITION(mStrafeSpeed, mMode.mValue != Mode::Legit);


        VISIBILITY_CONDITION(mSpeed, mMode.mValue == Mode::Friction || mMode.mValue == Mode::Legit);
        VISIBILITY_CONDITION(mFriction, mMode.mValue == Mode::Friction || mMode.mValue == Mode::Legit);
        VISIBILITY_CONDITION(mTimerBoost, mMode.mValue == Mode::Friction || mMode.mValue == Mode::Legit);
        VISIBILITY_CONDITION(mTimerSpeed, mMode.mValue == Mode::Friction && mTimerBoost.mValue || mMode.mValue == Mode::Legit);
        VISIBILITY_CONDITION(mFastFall, mMode.mValue == Mode::Friction || mMode.mValue == Mode::Legit);
        VISIBILITY_CONDITION(mFallTicks, mMode.mValue == Mode::Friction && mFastFall.mValue != FastfallMode::None || mMode.mValue == Mode::Legit && mFastFall.mValue != FastfallMode::None);
        VISIBILITY_CONDITION(mFallSpeed, mMode.mValue == Mode::Friction && mFastFall.mValue != FastfallMode::None || mMode.mValue == Mode::Legit && mFastFall.mValue != FastfallMode::None);
        VISIBILITY_CONDITION(mFastFall2, mMode.mValue == Mode::Friction && mFastFall.mValue != FastfallMode::None || mMode.mValue == Mode::Legit && mFastFall.mValue != FastfallMode::None);
        VISIBILITY_CONDITION(mFallTicks2, mMode.mValue == Mode::Friction && mFastFall.mValue != FastfallMode::None && mFastFall2.mValue || mMode.mValue == Mode::Legit && mFastFall.mValue != FastfallMode::None && mFastFall2.mValue);
        VISIBILITY_CONDITION(mFallSpeed2, mMode.mValue == Mode::Friction && mFastFall.mValue != FastfallMode::None && mFastFall2.mValue || mMode.mValue == Mode::Legit && mFastFall.mValue != FastfallMode::None && mFastFall2.mValue);
        VISIBILITY_CONDITION(mJumpType, mMode.mValue == Mode::Friction || mMode.mValue == Mode::Legit);
        VISIBILITY_CONDITION(mJumpHeight, mMode.mValue == Mode::Friction && mJumpType.mValue != JumpType::None || mMode.mValue == Mode::Legit && mJumpType.mValue != JumpType::None);
        VISIBILITY_CONDITION(mApplyNetskip, mMode.mValue == Mode::Friction || mMode.mValue == Mode::Legit);

        mNames = {
            {Lowercase, "speed"},
            {LowercaseSpaced, "speed"},
            {Normal, "Speed"},
            {NormalSpaced, "Speed"}
        };

        gFeatureManager->mDispatcher->listen<PacketInEvent, &Speed::onPacketInEvent>(this);
    }

    std::map<EffectType, uint64_t> mEffectTimers = {};
    float mDamageBoostVal = 1.f;
    bool mDamageTimerApplied = false;
    bool mClip = false;
    uint64_t mLastAvoidCheck = 0;

    void onEnable() override;
    void onDisable() override;
    void onRunUpdateCycleEvent(class RunUpdateCycleEvent& event);
    bool tickSwiftness();
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    void tickLegit(Actor* player);
    void tickFriction(Actor* player);
    void tickFrictionPreset(FrictionPreset& preset);

    std::string getSettingDisplay() override
    {
        return mMode.mValues[mMode.as<int>()];
    }
};
