//
// Created by Flash on 7/11/2023.
//

#include "NoPacket.h"


Vector3 NoPacket::LastPos = Vector3(0, 0, 0);

void NoPacket::OnEnable(){
    if (!Minecraft::ClientInstance->ThePlayer()) return;
    LastPos = *Minecraft::ClientInstance->ThePlayer()->getPos();
}

void NoPacket::OnDisable(){
    LastPos = Vector3(0, 0, 0);
}

void NoPacket::OnPacket(class LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    if (CancelMovingPackets.Enabled){

        if (packet->getId() == (int)PacketID::PlayerAuthInput ||
        packet->getId() == (int)PacketID::MovePlayer)
            *canceled = true;

    } else {
        *canceled = true;
    }
}