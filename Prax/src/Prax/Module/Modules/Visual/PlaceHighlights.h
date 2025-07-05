#pragma once
//
// Created by vastrakai on 9/28/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>

class PlaceHighlights : public ModuleBase<PlaceHighlights> {
public:
    ModeSetting DrawMethod = ModeSetting("Draw Method", "How to draw the placed blocks", { "Outline", "Fill" }, 0);
    NumberSetting SecondsToRender = NumberSetting("Seconds to Render", "How many seconds to render placed blocks for", 1.f, 10.f, 2.5f, 0.01f);
    NumberSetting MaxOpacity = NumberSetting("Max Opacity", "The maximum opacity of the placed blocks", 0.f, 1.f, 0.5f, 0.01f);

    PlaceHighlights() : ModuleBase("PlaceHighlights", "Highlights blocks that you place", "Visual", 0, false) {
        DrawMethod.Display = true;
        AddSetting(&DrawMethod);
        AddSetting(&SecondsToRender);
        AddSetting(&MaxOpacity);
        RenderOnDisabled = true;
    }

    static std::map<uint64_t, Vector3> LastPlaceMap;

    void OnTick() override;
    void OnDisable() override;
    void OnEnable() override;
    void OnRender() override;
};