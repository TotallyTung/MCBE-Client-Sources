//
// Created by vastrakai on 9/24/2023.
//

#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include "NoFall.h"


bool everyOtherTickLol = false;

void NoFall::OnPacket(class LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    everyOtherTickLol = !everyOtherTickLol;
    if (everyOtherTickLol) return;
    if(packet->getId() == (int)PacketID::PlayerAuthInput){
        auto paip = (PlayerAuthInputPacket*)packet;

        paip->velocity.y = -0.0784000015258789f;
    }
}