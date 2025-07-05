#pragma once
//
// Created by vastrakai on 9/24/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>

class NoFall : public ModuleBase<NoFall> {
public:
    ModeSetting Mode = ModeSetting("Mode", "The mode to use", {"Sentinel"}, 0);
    NoFall() : ModuleBase("NoFall", "Prevents fall damage", "Player", 0, false) {
        Mode.Display = true;
        AddSetting(&Mode);
    }
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled);
};