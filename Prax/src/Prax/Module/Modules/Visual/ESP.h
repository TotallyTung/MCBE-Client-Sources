#pragma once
//
// Created by vastrakai on 8/20/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>

class ESP : public ModuleBase<ESP> {
public:
    enum class RGBMode {
        Disabled,
        Themed
    };
    BoolSetting RenderOnSelf = BoolSetting("Render on self", "Render on self", false);
    BoolSetting ShowFriends = BoolSetting("Show friends", "Show friends", true);
    ModeSetting RGBMode = ModeSetting("RGB", "RGB", {"Disabled", "Themed"}, 1);
    ModeSetting Mode = ModeSetting("Mode", "The mode to use", {"Image", "2D", "3D", "Outline"}, 0);
    BoolSetting HighlightBots = BoolSetting("Highlight bots", "Highlight bots", false);

    ESP() : ModuleBase("ESP", "Shows entities", "Visual", 0, false) {
        AddSetting(&Mode);
        AddSetting(&RGBMode);
        AddSetting(&RenderOnSelf);
        AddSetting(&ShowFriends);
        AddSetting(&HighlightBots);
        Mode.Display = true;
    }

    std::map<Actor*,AABB> GetAABBs();
    void OnRender() override;

};