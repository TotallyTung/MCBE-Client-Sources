//
// Created by vastrakai on 9/17/2023.
//

#include <Prax/SDK/Game/Network/Other/MovePlayerPacket.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include "Franky.h"

#include <Prax/Hook/Hooks/PacketSender.h>


void Franky::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (packet->getId() == (int)PacketID::MovePlayer) {
        auto mpp = std::reinterpret_pointer_cast<MovePlayerPacket>(packet);
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;


        *cancel = true;

        auto newPaip = player->getInputPacket();
        newPaip->velocity = Vector3(0.f, -0.0784000015258789f, 0.f);
        newPaip->position = mpp->position;
        newPaip->rotation = mpp->rotation;
        newPaip->headYaw = mpp->headYaw;
        player->addChatMessage("Sent silentAccept at pos " + std::to_string(mpp->position.x) + ", " + std::to_string(mpp->position.y) + ", " + std::to_string(mpp->position.z) + " and rot " + std::to_string(mpp->rotation.x) + ", " + std::to_string(mpp->rotation.y), 0);
        bool t = false;
        PacketSender::HandleAuthInputTick(newPaip.get(), &t);
        Minecraft::ClientInstance->packetSender->sendToServer(newPaip.get());


    }
}

void Franky::OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        auto paip = (PlayerAuthInputPacket*)packet;
        int invalidTicks = (int)InvalidTicks.Value;
        int ticks = paip->ticksAlive;
        if (ticks % invalidTicks != 0) {
            // cancel this packet
            *canceled = true;

            // send a new packet
            if (InvalidMode == Blank)
            {
                auto newPacket = MinecraftPackets::CreatePacket<PlayerAuthInputPacket>();
                bool t = false;
                PacketSender::HandleAuthInputTick(newPacket.get(), &t);
                Minecraft::ClientInstance->packetSender->sendToServer(newPacket.get());
                player->addChatMessage("Sent blank packet", 0);
            } else if (InvalidMode == PosZero) {
                auto newPacket = player->getInputPacket();
                newPacket->position = Vector3(0, 0, 0);
                bool t = false;
                PacketSender::HandleAuthInputTick(newPacket.get(), &t);
                Minecraft::ClientInstance->packetSender->sendToServer(newPacket.get());
                player->addChatMessage("Sent posZero packet", 0);
            } else if (InvalidMode == PosMax) {
                auto newPacket = player->getInputPacket();
                newPacket->position = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
                bool t = false;
                PacketSender::HandleAuthInputTick(newPacket.get(), &t);
                Minecraft::ClientInstance->packetSender->sendToServer(newPacket.get());
                player->addChatMessage("Sent posMax packet", 0);
            }
        } else {
            player->addChatMessage("Sent valid packet", 0);
        }
    }
}

void Franky::OnEnable(){

}

void Franky::OnDisable(){
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
}
