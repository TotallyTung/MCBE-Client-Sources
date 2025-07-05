//
// Created by vastrakai on 5/10/2024.
//

#include "NoScoreboard.h"
#include <Prax/SDK/Game/Network/Other/SetScorePacket.h>

void NoScoreboard::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel)
{
    if (packet->getId() == (int)PacketID::SetScore)
    {
        auto scorePacket = std::reinterpret_pointer_cast<SetScorePacket>(packet);
        if (scorePacket->mType == ScorePacketType::Change)
            scorePacket->mType = ScorePacketType::Remove;
    }
}
