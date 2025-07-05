//
// Created by vastrakai on 1/9/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class Reach : public ModuleBase<Reach> {
public:
    // i kinda want to have a combo setting for this that would have a Enabled and a Value setting in one
    BoolSetting CombatReachEnabled = BoolSetting("Enable Combat Reach", "Whether or not to modify combat reach", true);
    NumberSetting CombatReach = NumberSetting("Combat Reach", "The reach to set", 3.f, 10.f, 3.f, 0.01f);
    BoolSetting BlockReachEnabled = BoolSetting("Enable Block Reach", "Whether or not to modify block reach", true);
    NumberSetting BlockReach = NumberSetting("Block Reach", "The reach to set", 5.f, 50.f, 6.f, 0.01f);

    Reach() : ModuleBase("Reach", "Modifies your reach", "Combat", 0, false) {
        AddSetting(&CombatReachEnabled);
        AddSetting(&CombatReach);
        AddSetting(&BlockReachEnabled);
        AddSetting(&BlockReach);
    }

    static float* ReachPtr;

    void OnInitialize() override;
    void OnTick() override;
    void OnEnable() override;
    void OnDisable() override;
};
