//
// Created by vastrakai on 5/6/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>

class AutoDodge : public ModuleBase<AutoDodge> {
public:
    ModeSetting Mode = ModeSetting("Mode", "The server to dodge in", { "Hive" }, 0);
    BoolSetting DodgeEuropa = BoolSetting("Dodge Europa", "Dodge Europa", false);
    BoolSetting DodgeTurmoil = BoolSetting("Dodge Turmoil", "Dodge Turmoil", false);

    AutoDodge() : ModuleBase("AutoDodge", "Automatically dodges certain maps on Hive Skywars (relies on AutoQueue)", "Misc", 0, false) {
        AddSetting(&Mode);
        Mode.Display = true;
        AddSetting(&DodgeEuropa);
        AddSetting(&DodgeTurmoil);
    }

    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;

};