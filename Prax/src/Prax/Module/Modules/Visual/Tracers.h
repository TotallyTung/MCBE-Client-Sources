//
// Created by Flash on 9/11/2023.
//

#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>

class Tracers : public ModuleBase<Tracers> {
public:
    enum class RGBMode {
        Disabled,
        Themed
    };
    ModeSetting RGB = ModeSetting("RGB", "The mode to use", {"Disabled", "Themed"}, 1);
    BoolSetting ShowFriends = BoolSetting("Show friends", "Show friends", true);
    ModeSetting Mode = ModeSetting("Start Position", "Where the Tracers start rendering", {"Bottom", "Middle", "Top"}, 0);

    Tracers() : ModuleBase("Tracers", "Shows a line to entities", "Visual", 0, false) {
        AddSetting(&RGB);
        AddSetting(&ShowFriends);
        AddSetting(&Mode);
    }

    std::map<Actor*, Vector3> GetHeadLocations();
    void OnRender() override;


};
