//
// Created by vastrakai on 5/6/2024.
//

#include "AutoDodge.h"

#include <Prax/SDK/Game/Network/Other/TextPacket.h>

#include "AutoQueue.h"

void AutoDodge::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel)
{
    if (packet->getId() != (int)PacketID::Text) return;

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    auto tp = std::reinterpret_pointer_cast<TextPacket2>(packet);
    //§b§l» §r§eMapName §7won with §fNUMBER §7votes!
    if (String::StartsWith(tp->message, "§b§l» §r§e"))
    {
        // Find the text in between the §e and §7
        auto mapName = tp->message.substr(8, tp->message.find(" §7") - 8);
        mapName = String::RemoveColorCodes(mapName);
        mapName = String::Trim(mapName);
        Logger::Write("AutoDodge", "Map: '" + mapName + "' won", Logger::Debug);
        // No need to check mode because we only queue in Hive Skywars
        bool requeue = false;

        if (DodgeEuropa.Enabled && mapName == "Europa")
        {
            player->addChatMessage("&6[AutoDodge]&f Mannn this map SUCKSSSS");
            requeue = true;
        }
        else if (DodgeTurmoil.Enabled && mapName == "Turmoil")
        {
            player->addChatMessage("&6[AutoDodge]&f Mannn this map SUCKSSSS");\
            requeue = true;
        }

        if (!requeue) return;

        AutoQueue::queueForGame = true;
        AutoQueue::lastQueueTime = Misc::GetCurrentMs();

    }
}
