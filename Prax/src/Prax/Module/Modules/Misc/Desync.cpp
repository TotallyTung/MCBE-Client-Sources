//
// Created by vastrakai on 11/5/2023.
//


#include <iostream>
#include <Prax/Utils/System/KeyManager.h>
#include "Desync.h"

#include <Prax/Hook/Hooks/PacketSender.h>

std::vector<std::shared_ptr<PlayerAuthInputPacket>> Desync::QueuedPackets;

bool Desync::Ticking = false;
bool Desync::Visualize = false;

Vector3 Desync::LastPos = Vector3(0.f, 0.f, 0.f);

void Desync::OnEnable() {
    Ticking = true;
    Visualize = VisualizeSetting.Enabled;
}

void Desync::OnDisable() {
    Ticking = false;

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player)
    {
        QueuedPackets.clear();
        return;
    }

    if (QueuedPackets.size() > 0)
    {
        for (auto &packet : QueuedPackets) {
            bool t = false;
            PacketSender::HandleAuthInputTick(packet.get(), &t);
            Minecraft::ClientInstance->packetSender->sendToServer(packet.get());
        }

        player->addChatMessage("&6[Desync] &aSent &7" + std::to_string(QueuedPackets.size()) + " &apackets", true);
    }

}

void Desync::OnTick() {
    Visualize = VisualizeSetting.Enabled;
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    int i = 0;
    if (QueuedPackets.size() >= Ticks.Value)
    {
        for (auto &packet : QueuedPackets) {
            bool t = false;
            PacketSender::HandleAuthInputTick(packet.get(), &t);
            Minecraft::ClientInstance->packetSender->sendToServer(packet.get());
        }

        player->addChatMessage("&6[Desync] &aSent &7" + std::to_string(QueuedPackets.size()) + " &apackets", true);
        QueuedPackets.clear();
        LastPos = *player->getPos();
    }
}

void Desync::OnPacket(class LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto *paip = (PlayerAuthInputPacket *) packet;

        if (Ticking) {
            auto cloned = MinecraftPackets::CreatePacket<PlayerAuthInputPacket>();
            cloned->CopyFrom(paip);
            QueuedPackets.push_back(cloned);
            *canceled = true;
            return;
        }
    }
}
