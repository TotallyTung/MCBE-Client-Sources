#pragma once

//
// Created by Aether on 9/01/2024
//

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class AimAssist : public ModuleBase<AimAssist> {
public:
    NumberSetting DistanceSetting = NumberSetting("Distance", "The distance at which it starts aiming", 0, 20, 20,0.01);
    NumberSetting SmoothFactor = NumberSetting("Smoothing", "Slows down the aim speed", 0, 2, 1, 0.01);
    NumberSetting MinimumFOV = NumberSetting("Minimum FOV", "The minimum FOV to start aiming", 0, 180, 0, 0.01);

    AimAssist() : ModuleBase("AimAssist", "Helps you aim to entities", "Combat") {
        AddSetting(&DistanceSetting);
        AddSetting(&SmoothFactor);
        AddSetting(&MinimumFOV);
    };

    void OnTick() override;

    void OnRender() override;

private:
    static float AngleDifference(float a, float b) {
        float delta = fmodf(b - a, 360);
        return delta;
    }

    static bool IsEntityInFov(float angle, Vector2 currentAngles, Vector2 targetAngles);

    bool IsValid(Actor *a);
};