//
// Created by vastrakai on 1/9/2024.
//


#pragma once
#include <Prax/Module/Module.h>

class ServerSneak : public ModuleBase<ServerSneak> {
public:
    ServerSneak() : ModuleBase("ServerSneak", "Silently sneak on servers", "Movement") {}

    void OnEnable() override;
    void OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled) override;
};
