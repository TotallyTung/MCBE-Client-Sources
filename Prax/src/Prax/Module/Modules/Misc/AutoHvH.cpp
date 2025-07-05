//
// Created by vastrakai on 4/14/2024.
//

#include "AutoHvH.h"

#include <Prax/Command/Commands/CheckAllStats.h>

void AutoHvH::OnGameStart()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    player->addChatMessage("§6[AutoHvH] &r&aGame started!");

    std::thread t(CheckAllStats::Exec);
    t.detach();
}

void AutoHvH::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();


    static bool lastImmobileState = false;
    bool immobile = player->isImmobile();

    // if the we are no longer immobile and the dimension change was less than 11 seconds ago then call OnGameStart
    if (!immobile && lastImmobileState && (Misc::GetCurrentMs() - LastDimensionChange) < 11000)
    {
       OnGameStart();
    }

    lastImmobileState = immobile;
}

void AutoHvH::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel)
{
    if (packet->getId() == static_cast<int>(PacketID::ChangeDimension))
        LastDimensionChange = Misc::GetCurrentMs();
}
