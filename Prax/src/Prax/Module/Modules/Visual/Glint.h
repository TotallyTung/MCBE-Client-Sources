//
// Created by vastrakai on 12/21/2023.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>

class Glint : public ModuleBase<Glint> {
public:
    ModeSetting Mode = ModeSetting("Mode", "Mode of the glint", { "Additive", "Set" }, 0);
    NumberSetting Saturation = NumberSetting("Saturation", "Saturation of the glint", 0.f, 1.f, 1.f, 0.1f);

    Glint() : ModuleBase("Glint", "Edits the enchantment glint", "Visual", 0, false) {
        AddSetting(&Mode);
        AddSetting(&Saturation);
    }

};