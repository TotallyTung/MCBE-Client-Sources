//
// Created by vastrakai on 2/10/2024.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>

class SelectionColor : public ModuleBase<SelectionColor> {
public:
    enum class ColorMode {
        Rainbow,
        White
    };
    ModeSetting Color = ModeSetting("Color", "The color of the selection box.", { "Rainbow", "White"}, 0);
    NumberSetting Speed = NumberSetting("Speed", "The speed of the rainbow.", 1, 20, 3, 0.1);
    NumberSetting Saturation = NumberSetting("Saturation", "The saturation of the color.", 0, 1, 1, 0.1);

    SelectionColor() : ModuleBase("SelectionColor", "Change the color of the selection box.", "Visual", 0, false) {
        AddSetting(&Color);
        AddSetting(&Speed);
        AddSetting(&Saturation);
    }

    /* Static variables */

    /* Module functions */

    static std::vector<uint8_t> OgBytes;
    /* Module overrides */
    void OnEnable() override;
    void OnDisable() override;
};
