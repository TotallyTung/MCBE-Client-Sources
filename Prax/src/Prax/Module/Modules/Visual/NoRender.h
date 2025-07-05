//
// Created by vastrakai on 2/4/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/BoolSetting.h>

class NoRender : public ModuleBase<NoRender>
{
public:
    BoolSetting Particles = BoolSetting("Particles", "Disable particles", false);

    NoRender() : ModuleBase("NoRender", "Remove visual effects", "Visual", 0, false) {
        AddSetting(&Particles);
    }

};