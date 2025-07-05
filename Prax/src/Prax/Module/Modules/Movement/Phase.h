//
// Created by vastrakai on 10/19/2023.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/BoolSetting.h>

class Phase : public ModuleBase<Phase> {
public:
    ModeSetting Mode = ModeSetting("Mode", "The mode to use for phase.\n"
                                         "Horizontal: Only phase through blocks on your x and z axis\n"
                                         "Vertical: Only phase through blocks on your y axis by holding shift or space\n",
                                         {"Horizontal", "Vertical"}, 0);
    NumberSetting VerticalSpeed = NumberSetting("Vertical Speed", "The speed to phase vertically", 0.01f, 10.f, 2.f, 0.1f);
#ifndef __RELEASEFREE__
    BoolSetting Blink = BoolSetting("Blink", "Blink when phasing", false);
#endif

    Phase() : ModuleBase("Phase", "Allows you to phase through walls", "Movement", 0, false)
    {
        AddSetting(&Mode);
        AddSetting(&VerticalSpeed);
#ifndef __RELEASEFREE__
        AddSetting(&Blink);
#endif
    }

    void OnEnable() override;
    void OnTick() override;
    void OnDisable() override;
    void OnRender() override;
};
