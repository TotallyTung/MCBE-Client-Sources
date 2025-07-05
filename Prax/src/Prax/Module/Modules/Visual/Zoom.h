//
// Created by vastrakai on 12/27/2023.
//


#pragma once

#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Module.h>

class Zoom : public ModuleBase<Zoom> {
public:
    NumberSetting ZoomAmount = NumberSetting("Target FOV", "How much to zoom in", 0, 100, 30, 1);
    BoolSetting Smooth = BoolSetting("Smooth", "Smoothly zoom in", true);
    NumberSetting SmoothSpeed = NumberSetting("Smoothing Speed", "How much smoothing to apply", 0, 100, 15, 1, &Smooth);
    BoolSetting ScrollZoom = BoolSetting("Scroll Zoom", "Allows you to zoom in/out with the scroll wheel", true);
    NumberSetting ScrollZoomAmount = NumberSetting("Scroll Zoom Amount", "How much to zoom in/out with the scroll wheel", 0, 100, 7.5f, 1, &ScrollZoom);

    Zoom() : ModuleBase("Zoom", "Allows you to zoom in", "Visual", 0, false) {
        EnableWhileHeld = true;
        RenderOnDisabled = true;
        AddSetting(&ZoomAmount);
        AddSetting(&Smooth);
        AddSetting(&SmoothSpeed);
        AddSetting(&ScrollZoom);
        AddSetting(&ScrollZoomAmount);
    }

    static float OldFOV;
    static bool Rendering;

    static float TargetFOV;
    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;
    void OnRender() override;
    void OnEject() override;
    void OnMouseInput(void* Instance, int mouseButton, int isDown, __int16 mouseX, __int16 mouseY, __int64 a1, __int64 a2, __int64 a3, bool* cancel) override;
};