//
// Created by vastrakai on 1/19/2024.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class HealthAlert : public ModuleBase<HealthAlert> {
public:
    NumberSetting MinHealth = NumberSetting("Min Health", "The minimum health to alert at", 0, 20, 4, 0.5);

    HealthAlert() : ModuleBase("HealthAlert", "Alerts you when your health is low", "Player", 0, false) {
        AddSetting(&MinHealth);
    }

    void OnTick() override;


};
