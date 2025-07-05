//
// Created by vastrakai on 1/9/2024.
//

#include "ServerSneak.h"

#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>


void ServerSneak::OnEnable()
{
}

void ServerSneak::OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled)
{
    if (packet->getId() == (int)PacketID::PlayerAuthInput)
    {
        auto* paip = (PlayerAuthInputPacket*)packet;
        paip->inputFlags |= AuthInputAction::START_SNEAKING | AuthInputAction::SNEAKING | AuthInputAction::SNEAK_DOWN;
        paip->inputFlags &= ~AuthInputAction::STOP_SNEAKING;

    }
}
