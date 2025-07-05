//
// Created by vastrakai on 2/23/2024.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class AutoJump : public ModuleBase<AutoJump> {
public:
    NumberSetting TimerBoost = NumberSetting("Timer Boost", "Timer boost value", 1.f, 40.f, 20.f, 0.1f);
    NumberSetting HeightLoss = NumberSetting("Height Loss", "The amount of height to lose per airjump", 0.f, 2.f, 0.5f, 0.1f);
    NumberSetting JumpDelay = NumberSetting("Jump Delay", "The amount of time to wait before jumping (in seconds)", 0.f, 1.f, 0.2f, 0.1f);
    BoolSetting ResetOnGround = BoolSetting("Reset On Ground", "Reset the airjump height when on ground", true);

    AutoJump() : ModuleBase("AutoJump", "Automatically airjumps (fly method)", "Movement", 0, false)
    {
        AddSetting(&TimerBoost);
        AddSetting(&HeightLoss);
        AddSetting(&JumpDelay);
        AddSetting(&ResetOnGround);
    }

    /* Static variables */
    static float EnableY;
    static uint64_t LastJump;

    /* Module functions */
    void Jump();

    /* Module overrides */
    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;

};
