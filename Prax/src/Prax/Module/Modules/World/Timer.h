#pragma once

#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include "../../Module.h"

class Timer : public ModuleBase<Timer> {
public:

    NumberSetting TimerSetting = NumberSetting("Timer", "The tick speed for the game", 0, 120, 20, 0.01);

    Timer() : ModuleBase("Timer", "Changes how fast the tick speed is", "World", 0, false) {
        TimerSetting.Display = true;
        AddSetting(&TimerSetting);
    }

    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;

};