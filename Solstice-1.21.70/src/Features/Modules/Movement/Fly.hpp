#pragma once
//
// Created by vastrakai on 6/30/2024.
//

#include <Features/Modules/Setting.hpp>

class Fly : public ModuleBase<Fly> {
public:
    enum class Mode {
        Motion,
        Elytra,
        Pregame,
        Jump,
        Vertical,
    };

    EnumSettingT<Mode> mMode = EnumSettingT("Mode", "The mode of the fly", Mode::Motion,  "Motion", "Elytra", "Pregame", "Jump", "Vertical");
    BoolSetting mApplyGlideFlags = BoolSetting("Apply Glide Flags", "Applies glide flags to the player", true); // Exclusive to Motion mode
    NumberSetting mSpeed = NumberSetting("Speed", "The speed of the fly", 5.6f, 0.f, 20.f, 0.1f);

    // Exclusively for Jump mode
    BoolSetting mSpeedFriction = BoolSetting("Speed Friction", "Applies friction to speed", true);
    NumberSetting mFriction = NumberSetting("Friction", "The amount of friction to apply", 0.975f, 0.f, 1.f, 0.01f);
    NumberSetting mHeightLoss = NumberSetting("Height Loss", "The amount of height to lose per airjump", 0.5f, 0.f, 2.f, 0.01f);
    NumberSetting mJumpDelay = NumberSetting("Jump Delay", "The amount of time to wait before jumping (in seconds)", 0.2f, 0.f, 1.f, 0.01f);
    BoolSetting mDamageOnly = BoolSetting("Damage Only", "Only fly when you take damage", false);
    NumberSetting mFlyTime = NumberSetting("Fly Time", "The amount of time to fly after taking damage (in seconds)", 1.f, 0.f, 10.f, 0.01f);
    BoolSetting mResetOnGround = BoolSetting("Reset On Ground", "Reset the airjump height when on ground", true);
    BoolSetting mResetOnDisable = BoolSetting("Reset On Disable", "Resets velocity when you disable", true);
    BoolSetting mDebug = BoolSetting("Debug", "Displays debug messages", false);

    NumberSetting mStep = NumberSetting("Step", "The step of the vertical fly", 2.50, 1, 5, 0.01);

    // Applies to all modes
    BoolSetting mTimerBoost = BoolSetting("Timer Boost", "Whether to boost the timer", false);
    NumberSetting mTimerBoostValue = NumberSetting("Timer Boost Value", "The new timer value", 1.f, 0.01f, 60.f, 0.01f);

    Fly() : ModuleBase("Fly", "Allows you to fly", ModuleCategory::Movement, 0, false) {
        addSettings(
            &mMode,
            &mApplyGlideFlags,
            &mSpeed,
            // Jump mode settings
            &mSpeedFriction,
            &mFriction,
            &mHeightLoss,
            &mJumpDelay,
            &mDamageOnly,
            &mFlyTime,
            &mResetOnGround,
            &mResetOnDisable,
            &mDebug,

            &mStep,

            &mTimerBoost,
            &mTimerBoostValue
        );
        VISIBILITY_CONDITION(mApplyGlideFlags, mMode.mValue == Mode::Motion);
        VISIBILITY_CONDITION(mSpeed, mMode.mValue == Mode::Motion || mMode.mValue == Mode::Jump || mMode.mValue == Mode::Elytra);

        VISIBILITY_CONDITION(mHeightLoss, mMode.mValue == Mode::Jump);
        VISIBILITY_CONDITION(mJumpDelay, mMode.mValue == Mode::Jump);
        VISIBILITY_CONDITION(mSpeedFriction, mMode.mValue == Mode::Jump);
        VISIBILITY_CONDITION(mFriction, mMode.mValue == Mode::Jump && mSpeedFriction.mValue);
        VISIBILITY_CONDITION(mDamageOnly, mMode.mValue == Mode::Jump);
        VISIBILITY_CONDITION(mFlyTime, mMode.mValue == Mode::Jump && mDamageOnly.mValue);

        VISIBILITY_CONDITION(mDebug, mMode.mValue == Mode::Jump || mMode.mValue == Mode::Vertical);
        VISIBILITY_CONDITION(mResetOnGround, mMode.mValue == Mode::Jump);
        VISIBILITY_CONDITION(mResetOnDisable, mMode.mValue == Mode::Jump);

        VISIBILITY_CONDITION(mStep, mMode.mValue == Mode::Vertical);

        VISIBILITY_CONDITION(mTimerBoostValue, mTimerBoost.mValue);


        mNames = {
            {Lowercase, "fly"},
            {LowercaseSpaced, "fly"},
            {Normal, "Fly"},
            {NormalSpaced, "Fly"}
        };

        gFeatureManager->mDispatcher->listen<PacketInEvent, &Fly::onPacketInEvent>(this);

    }

    float mCurrentY = 0.f;
    uint64_t mLastJump = 0;
    float mCurrentFriction = 1.f;
    uint64_t mLastDamage = 0;
    float topY = 0;


    void onEnable() override;
    void onDisable() override;
    void displayDebug(const std::string& message) const;
    void onBaseTickEvent(class BaseTickEvent& event);
    bool tickJump(Actor* player);
    void jump();
    void onPacketOutEvent(class PacketOutEvent& event) const;
    void onPacketInEvent(class PacketInEvent& event);

    std::string getSettingDisplay() override
    {
        return mMode.mValues[mMode.as<int>()];
    }
};