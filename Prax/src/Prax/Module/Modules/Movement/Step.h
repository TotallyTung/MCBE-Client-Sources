//
// Created by vastrakai on 1/13/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/SDK/Game/Entity/Components/MaxAutoStepComponent.h>
#include <Prax/Module/Settings/ModeSetting.h>


class Step : public ModuleBase<Step> {
public:
    enum StepMode {
        Vanilla,
        Clip
    };

    ModeSetting Mode = ModeSetting("Mode", "How you should step", {"Vanilla", "Clip"}, 0);
    NumberSetting StepHeight = NumberSetting("Step Height", "Height of the step", 0.0, 10.f, 0.1f, 0.1f);

    Step() : ModuleBase("Step", "Step up blocks", "Movement", 0, false)
    {
        AddSetting(&Mode);
        AddSetting(&StepHeight);
    }

    static float OldStep;

    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;
    void OnRender() override;

    Vector3 GetCollidingBlock();
};