#pragma once
//
// Created by vastrakai on 9/1/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/BoolSetting.h>

class AirJump : public ModuleBase<AirJump> {
public:

    AirJump() : ModuleBase("AirJump", "Allows you to jump in mid-air", "Movement", 0, false) {
    };

    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
};