#pragma once
//
// Created by vastrakai on 8/23/2023.
//
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Module.h>

class Spider : public ModuleBase<Spider> {
public:

    enum SpiderMode {
        Vanilla,
        Flareon,
        Clip
    };
    ModeSetting Mode = ModeSetting("Mode", "How you should climb", {"Vanilla", "Flareon", "Clip"}, 0);
    NumberSetting SpeedSetting = NumberSetting("Speed", "How fast you should go", 0, 10, 3.f, 0.01);

    Spider() : ModuleBase("Spider", "Lets you climb walls", "Movement", 0, false) {
        AddSetting(&Mode);
        AddSetting(&SpeedSetting);
    }

    static bool WasBlinking;
    static bool LastWasTicking;
    static bool LastVisualize;
    static int LastTicksToSkip;
    static int LastMsToSkip;

    void OnTick() override;
    void OnDisable() override;
};
