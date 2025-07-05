//
// Created by Flash on 7/30/2023.
//

#pragma once


#include <Prax/Hook/Hook.h>
#include <memory>
#include <Prax/SDK/Game/Network/Network.h>

class PlayerAuthInputPacket;

class PacketSender : public Hook {
public:
    PacketSender() : Hook("PacketSender") {}
    std::unique_ptr<Detour> PacketSenderDetour;
    static int CurrentTick;

    static void SendDetour(LoopbackPacketSender* _this, Packet* packet);
    static void HandleAuthInputTick(PlayerAuthInputPacket* paip, bool* cancel);

    void Init() override;
    void UnHook() override;

};
