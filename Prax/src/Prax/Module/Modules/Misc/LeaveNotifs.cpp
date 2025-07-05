//
// Created by vastrakai on 10/30/2023.
//

#include <Prax/UI/Notifications.h>
#include <Prax/SDK/Game/Network/Other/TextPacket.h>
#include <Prax/Utils/String.h>
#include <Prax/Module/Modules/Player/Friends.h>
#include "LeaveNotifs.h"

std::vector<std::string> oldPlayers = {};
std::map<uint64_t, std::string> lastChats = {};
uint64_t lastDimensionChangeTime = {};


void LeaveNotifs::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    std::vector<std::string> players = {};

    for (auto set : player->level->playerList) {
        players.push_back(set.second.name);
    }

    // If the dimension changed in the last 500ms, it was probably a teleport
    if (Misc::GetCurrentMs() - lastDimensionChangeTime < 500) {
        oldPlayers = players;
        return;
    }


    for (auto playa : players) {
        if (std::find(oldPlayers.begin(), oldPlayers.end(), playa) == oldPlayers.end() && ShowJoins.Enabled) {

            // Check for a join message in the last 100ms
            bool found = false;

            for (auto it = lastChats.rbegin(); it != lastChats.rend(); ++it) {
                if (String::ContainsIgnoreCase(it->second, playa) && it->first > Misc::GetCurrentMs() - 100) {
                    found = true;
                    break;
                }
            }

            // Check if the player is a friend

            if (Friends::IsFriend(playa)) playa = "§a" + playa + "§7";
            if (ShowInChat.Enabled && !found) player->displayClientMessage("§a§l» §r§7" + playa + " has joined.");
            if (ShowNotification.Enabled) Notifications::Notify(Prax::ClientName, playa + " has joined.", NotificationType::Enable, 3.0f);

        }
    }

    for (auto playa : oldPlayers) {
        if (std::find(players.begin(), players.end(), playa) == players.end()) {

            if (Friends::IsFriend(playa)) playa = "§a" + playa + "§7";
            bool found = false;

            for (auto it = lastChats.rbegin(); it != lastChats.rend(); ++it) {
                if (String::ContainsIgnoreCase(it->second, playa) && it->first > Misc::GetCurrentMs() - 600) {
                    if (ShowKicks.Enabled) {
                        if (ShowInChat.Enabled) player->displayClientMessage("§c§l» §r§7" + playa + " was kicked.");
                        if (ShowNotification.Enabled)
                            Notifications::Notify(Prax::ClientName, playa + " was kicked.", NotificationType::Warning, 3.0f);
                        found = true;
                    }
                    break;
                }
            }


            if (!found && ShowLeaves.Enabled) {
                if (ShowInChat.Enabled) player->displayClientMessage("§c§l» §r§7" + playa + " has left.");
                if (ShowNotification.Enabled)
                    Notifications::Notify(Prax::ClientName, playa + " has left.", NotificationType::Disable, 3.0f);
            }

        }
    }

    oldPlayers = players;

    // Remove chats from more than 2 second ago
    for (auto it = lastChats.begin(); it != lastChats.end();) {
        if (Misc::GetCurrentMs() - it->first > 2000) {
            it = lastChats.erase(it);
        } else {
            ++it;
        }
    }
}

void LeaveNotifs::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (packet->getId() == (int)PacketID::Text) {
        auto textPacket = std::reinterpret_pointer_cast<TextPacket>(packet);
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        lastChats[Misc::GetCurrentMs()] = textPacket->message.getText();
    } else if (packet->getId() == (int)PacketID::ChangeDimension) {
        lastDimensionChangeTime = Misc::GetCurrentMs();
    }
}
