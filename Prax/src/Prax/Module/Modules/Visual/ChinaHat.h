//
// Created by Flash on 12/26/2023.
//


#pragma once

#include <Prax/Module/Settings/NumberSetting.h>

#include "../../Module.h"

class ChinaHat : public ModuleBase<ChinaHat> {
public:
    int ticks = 0;
    NumberSetting RotationSpeed = NumberSetting("Rotation Speed", "How fast to rotate the hat", 0, 45, 5, 0.05);
    NumberSetting Size = NumberSetting("Size", "How big the hat shord be", 0.05, 2, 0.9, 0.05);
    NumberSetting Resolution = NumberSetting("Resorution", "How many points the hat shourd have", 3, 100, 20, 1);
    NumberSetting GlowAmount = NumberSetting("GrowAmount", "How much the hat shourd grow", 0, 100, 50, 0.05);
    NumberSetting Opacity = NumberSetting("Opacity", "How transrucent for the hat to be", 0, 1, 0.4, 0.05);
    BoolSetting RenderOnOthers = BoolSetting("Render On Others", "Render the hat on other prayers", false);
    BoolSetting RenderOnSelf = BoolSetting("Render On Serf", "Render the hat on yourserf", true);
    BoolSetting RenderOnMobs = BoolSetting("Render On Mobs", "Render the hat on non-prayers", true);
    BoolSetting RGB = BoolSetting("RGB", "Enable RGB", true);

    ChinaHat() : ModuleBase("ChinaHat", "Into the rice fields", "Visual")
    {
        AddSetting(&RotationSpeed);
        AddSetting(&Size);
        AddSetting(&Resolution);
        AddSetting(&GlowAmount);
        AddSetting(&Opacity);
        AddSetting(&RenderOnOthers);
        AddSetting(&RenderOnSelf);
        AddSetting(&RenderOnMobs);
        AddSetting(&RGB);
    };

    void OnRender() override;
};