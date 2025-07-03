//
// Created by Tozic on 9/16/2024.
//
#pragma once

#include <Features/Modules/Module.hpp>

class DebugFly : public ModuleBase<DebugFly> {
public:
    enum class SpeedMode {
        Static,
        Decrease,
        Loop,
        Wave,
        Test
    };

    enum class TimerMode {
        Static,
        Decrease,
        Loop,
        Wave
    };

    EnumSettingT<SpeedMode> mSpeedMode = EnumSettingT("Speed Mode", "The mode of the speed", SpeedMode::Static,  "Static", "Decrease", "Loop", "Wave", "Test");
    EnumSettingT<TimerMode> mTimerMode = EnumSettingT("Timer Mode", "The mode of the timer", TimerMode::Wave,  "Static", "Decrease", "Loop", "Wave");

    NumberSetting mSpeed = NumberSetting("Speed", "flying speed", 39.64594650268555, 1, 80, 0.1);
    NumberSetting mMinSpeed = NumberSetting("Min Speed", "min flying speed", 40.864864349365234, 0, 80, 0.1);
    NumberSetting mMaxSpeed = NumberSetting("Max Speed", "max flying speed", 51.24324417114258, 0, 80, 0.1);
    NumberSetting mSpeedMultiplier = NumberSetting("SpeedMultiplier", "increase horizontal speed by multiply", 2.1891891956329346, 0, 10, 0.1);
    NumberSetting mDelayTick = NumberSetting("DelayTick", "delay in ticks", 0, 0, 5, 0.1);
    NumberSetting mTimer = NumberSetting("Timer", "read the name", 0.36486488580703735, 0, 3, 0.1);
    NumberSetting mTimerMin = NumberSetting("TimerMin", "read the name", 0.30000001192092896, 0, 1, 0.1);
    NumberSetting mTimerMax = NumberSetting("TimerMax", "read the name", 0.5756756663322449, 0, 5, 0.1);
    NumberSetting mTimerMultiplier = NumberSetting("TimerMultiplier", "read the name", 2.1351351737976074, 0, 1, 0.1);
    NumberSetting mGlide = NumberSetting("Glide", "read the name", 1.921621561050415, 0, 10, 0.1);

    BoolSetting mTestGlide = BoolSetting("Glide multiplier", "read the name", true);
    BoolSetting mFullStop = BoolSetting("Full stop", "read the name", true);
    BoolSetting mDebug = BoolSetting("Debug", "read the name", true);


    DebugFly() : ModuleBase("DebugFly", "A 190 blocks fly (Config by default)", ModuleCategory::Movement, 0, false) {
        // Settings
        addSettings(
            &mSpeedMode,
            &mTimerMode,
            &mSpeed,
            &mMinSpeed,
            &mMaxSpeed,
            &mSpeedMultiplier,
            &mDelayTick,
            &mTimer,
            &mTimerMin,
            &mTimerMax,
            &mTimerMultiplier,
            &mGlide,
            &mTestGlide,
            &mFullStop,
            &mDebug
        );

        mNames = {
            {Lowercase, "debugfly"},
            {LowercaseSpaced, "debug fly"},
            {Normal, "DebugFly"},
            {NormalSpaced, "Debug Fly"}
        };
    }

    bool mJustEnabled = false;

    int mSpeedIndex = 0;
    int mTimerIndex = 0;

    bool mIsSpeedIncrease = false;
    bool mIsTimerIncrease = false;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};
