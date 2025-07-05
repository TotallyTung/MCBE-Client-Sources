#pragma once

#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include "../../Module.h"

class Fly : public ModuleBase<Fly> {
public:
    ModeSetting FlyModeSetting = ModeSetting("Mode", "The mode for fly", std::vector<std::string>{"Velocity", "Friction"}, 0);
    NumberSetting SpeedSetting = NumberSetting("Speed", "How fast you should go", 0, 120, 5.6f, 0.01);
    NumberSetting FrictionTicks = NumberSetting("Friction Ticks", "The amount of Ticks before friction is reset", 1, 120, 20, 0.01);
    NumberSetting SpeedFrictionMultiplier = NumberSetting("Speed Friction Multiplier", "The multiplier for friction", 0, 2, 0.98f, 0.01);
    NumberSetting GlideFrictionMultiplier = NumberSetting("Glide Friction Multiplier", "The multiplier for friction", 0, 2, 1.10f, 0.01);
    NumberSetting DistanceSetting = NumberSetting("Clip Distance", "The distance to fall before clippping up again\nSet to -1 to disable clipping", -0.01, 10, -0.01f, 0.01);
    BoolSetting ResetTicksOnClip = BoolSetting("Reset Ticks On Clip", "Resets the ticks when clipping", true);
    BoolSetting Glide = BoolSetting("Glide Down", "Makes you glide", false);
    NumberSetting GlideSpeed = NumberSetting("Glide", "How much you should glide", -2.f, 2.f, -0.30f, 0.01);
    NumberSetting TimerBoost = NumberSetting("Timer Boost", "The timer to set when this module is enabled\nSet to -0.1 to not override timer", -0.1, 120, 30, 0.01);


    Fly() : ModuleBase("Fly", "Lets you fly", "Movement", 0, false) {
        FlyModeSetting.Display = true;
        AddSetting(&FlyModeSetting);
        AddSetting(&SpeedSetting);
        AddSetting(&FrictionTicks);
        AddSetting(&SpeedFrictionMultiplier);
        AddSetting(&GlideFrictionMultiplier);
        AddSetting(&DistanceSetting);
        AddSetting(&ResetTicksOnClip);
        AddSetting(&Glide);
        AddSetting(&GlideSpeed);
        AddSetting(&TimerBoost);
    }

    static int Ticks;

    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;

};