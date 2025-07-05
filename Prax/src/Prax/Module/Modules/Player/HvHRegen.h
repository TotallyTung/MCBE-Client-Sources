//
// Created by vastrakai on 3/1/2024.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>

class HvHRegen : public ModuleBase<HvHRegen> {
public:
    enum class RegenMode {
        Single,
        Multi,
    };

    ModeSetting Mode = ModeSetting("Mode", "The mode to use for regen", {"Single", "Multi" }, 0);
    NumberSetting Distance = NumberSetting("Distance", "The max distance to mine blocks", 0, 20, 5.0f, 0.01);
    NumberSetting Delay = NumberSetting("Delay", "The delay between breaking blocks (in seconds)", 0, 1, 0.2f, 0.01);

    HvHRegen() : ModuleBase("HvHRegen", "Automatically breaks redstone on The Hive", "Player", 0, false) {
        AddSetting(&Mode);
        AddSetting(&Distance);
        AddSetting(&Delay);
    }

    void OnTick() override;



};
