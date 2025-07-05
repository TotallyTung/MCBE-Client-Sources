//
// Created by Flash on 9/12/2023.
//

#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>

class Watermark : public ModuleBase<Watermark> {
public:
    NumberSetting PositionFromCorner = NumberSetting("Position From Corner", "The distance from the corner to render the watermark", 0, 100, 40, 1);

    enum class VersionModes {
        Normal,
        Subtext
    };


    Watermark() : ModuleBase("Watermark", "Shows the watermark", "Visual", 0, true) {
        AddSetting(&PositionFromCorner);
    }

    void OnRender() override;
};
