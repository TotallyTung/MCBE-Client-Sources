#pragma once

#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include "src/Prax/Module/Module.h"

class ToggleSounds : public ModuleBase<ToggleSounds> {
public:
    ModeSetting SoundSetting = ModeSetting("Sound", "The sound to play when a module is toggled", std::vector<std::string>{"Lever", "Nice" ,"Click"}, 0);

    ToggleSounds() : ModuleBase("ToggleSounds", "Plays a sound when you toggle a module", "Misc", 0, false) {
        AddSetting(&SoundSetting);
    }

    void OnToggle(bool enabled);
};