//
// Created by vastrakai on 1/6/2024.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class AutoScale : public ModuleBase<AutoScale> {
public:
    NumberSetting GuiScale = NumberSetting("Gui Scale", "The scale of the gui", 1.f, 4.f, 2.f, 0.25f);

    AutoScale() : ModuleBase("AutoScale", "Automatically sets the gui scale", "Visual", 0, false) {
        AddSetting(&GuiScale);
    }

    void OnTick() override;
    static float OldScaleMultiplier;
    static Vector2 OldScaledResolution;
    static float OldGuiScale;
    void OnEnable() override;
    void OnDisable() override;
};
