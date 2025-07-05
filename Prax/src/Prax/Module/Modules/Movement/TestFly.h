//
// Created by vastrakai on 4/28/2024.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class TestFly : public ModuleBase<TestFly>
{
public:
    NumberSetting TimerBoost = NumberSetting("Timer Boost", "Timer boost value", 1.f, 40.f, 20.f, 0.1f);
    NumberSetting HeightLoss = NumberSetting("Height Loss", "The amount of height to lose per airjump", 0.f, 2.f, 0.5f, 0.1f);
    NumberSetting JumpDelay = NumberSetting("Jump Delay", "The amount of time to wait before jumping (in seconds)", 0.f, 1.f, 0.2f, 0.1f);
    BoolSetting ResetOnGround = BoolSetting("Reset On Ground", "Reset the airjump height when on ground", true);
    NumberSetting SpeedSetting = NumberSetting("Speed", "How fast you should go", 0, 20, 5.6f, 0.01);
    BoolSetting SpeedFriction = BoolSetting("Speed Friction", "Applies friction to speed", true);
    NumberSetting Friction = NumberSetting("Friction", "The amount of friction to apply", 0, 1, 0.975f, 0.01, &SpeedFriction);
    BoolSetting SetSpeedOnGround = BoolSetting("Set Speed On Ground", "Set speed when on ground", true);
    BoolSetting ResetOnDisable = BoolSetting("Reset On Disable", "Resets velocity when you disable", true);
    BoolSetting PauseDuringTimeframe = BoolSetting("Pause During Timeframe", "Pause the timer during the timeframe", false);
    NumberSetting PauseStart = NumberSetting("Pause Start", "The start of the pause timeframe", 0, 40, 0, 0.1, &PauseDuringTimeframe);
    NumberSetting PausedTimer = NumberSetting("Paused Timer", "The timer value to use when paused", 0, 40, 0, 0.1, &PauseDuringTimeframe);
    NumberSetting PauseEnd = NumberSetting("Pause End", "The end of the pause timeframe", 0, 40, 0, 0.1, &PauseDuringTimeframe);
    BoolSetting OnDamageOnly = BoolSetting("On Damage Only", "Only fly when you take damage", false);
    NumberSetting FlyTime = NumberSetting("Fly Time", "The amount of time to fly after taking damage", 0, 10, 2, 0.1, &OnDamageOnly);
    BoolSetting Debug = BoolSetting("Debug", "Debug mode", false);

    TestFly() : ModuleBase("TestFly", "Automatically airjumps (fly method)", "Movement", 0, false)
    {
        AddSetting(&TimerBoost);
        AddSetting(&HeightLoss);
        AddSetting(&JumpDelay);
        AddSetting(&ResetOnGround);
        AddSetting(&SpeedSetting);
        AddSetting(&SpeedFriction);
        AddSetting(&Friction);
        AddSetting(&SetSpeedOnGround);
        AddSetting(&ResetOnDisable);
        AddSetting(&PauseDuringTimeframe);
        AddSetting(&PauseStart);
        AddSetting(&PausedTimer);
        AddSetting(&PauseEnd);
        AddSetting(&OnDamageOnly);
        AddSetting(&FlyTime);
        AddSetting(&Debug);
    }

    /* Static variables */
    static float EnableY;
    static uint64_t LastJump;
    static float CurrentFriction;
    static uint64_t LastDamage;

    /* Module functions */
    void Jump();
    void AddDebugMessage(const std::string& message);

    /* Module overrides */
    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;
};
