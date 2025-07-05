//
// Created by vastrakai on 4/13/2024.
//

#include <Prax/UI/Notifications.h>
#include <Prax/SDK/Game/Network/Other/TextPacket.h>
#include "AutoSnipe.h"
#include "AutoQueue.h"

std::vector<std::string> AutoSnipe::Targets = {};

std::vector<std::string> getDaPlayerList() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    std::vector<std::string> playerList;

    for (auto const& [_, entry] : player->level->playerList) {
        playerList.emplace_back(entry.name);
    }

    return playerList;
}

void AutoSnipe::SendNotification(std::string message) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    player->addChatMessage("&6[AutoSnipe]&f " + message, true);
    Notifications::Notify("AutoSnipe", message, 5.f);
}

bool AutoSnipe::AnyTargetsFound() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto playerList = getDaPlayerList();

    for (auto const& player : playerList) {
        if (std::find(Targets.begin(), Targets.end(), player) != Targets.end()) {
            return true;
        }
    }

    return false;
}

bool queueForGame = false;
uint64_t lastQueueTime = 0;

void AutoSnipe::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();

    static std::vector<std::string> oldPlayerList;
    auto playerList = getDaPlayerList();
    bool targetNotifs = TargetNotifications.Enabled;

    if (queueForGame && Misc::GetCurrentMs() - lastQueueTime > 1000) {
        if (!AnyTargetsFound()) {
            AutoQueue::queueForGame = true;
            AutoQueue::lastQueueTime = Misc::GetCurrentMs();
            queueForGame = false;
        } else {
            queueForGame = false;
            SendNotification("&cQueue cancelled! Targets found!");
        }
    }


    std::vector<std::string> newPlayers = {};
    std::vector<std::string> lostPlayers = {};

    for (auto const& player : playerList) {
        if (std::find(oldPlayerList.begin(), oldPlayerList.end(), player) == oldPlayerList.end()) {
            // Continue if the player is not a target
            if (std::find(Targets.begin(), Targets.end(), player) == Targets.end()) {
                continue;
            }
            newPlayers.emplace_back(player);
        }
    }

    for (auto const& player : oldPlayerList) {
        if (std::find(playerList.begin(), playerList.end(), player) == playerList.end()) {
            // Continue if the player is not a target
            if (std::find(Targets.begin(), Targets.end(), player) == Targets.end()) {
                continue;
            }
            lostPlayers.emplace_back(player);
        }
    }

    if (targetNotifs) {
        for (auto const& player : newPlayers) {
            SendNotification("Found target: " + player);
            // Play target found sound
            Minecraft::ClientInstance->playUI("random.orb", Volume.Value, 1.f);
        }

        for (auto const& player : lostPlayers) {
            SendNotification("Lost target: " + player);
            // Play target lost sound (emphasized by lower pitch)
            Minecraft::ClientInstance->playUI("random.orb", Volume.Value, 0.5f);
        }
    }

    oldPlayerList = playerList;

    bool hasTargets = AnyTargetsFound();
    static bool lastHasTargets = false;

    if (!hasTargets && lastHasTargets) {
        SendNotification("&e[NOTICE]&c No more targets found!");
        // play low pitch note.bass
        Minecraft::ClientInstance->playUI("note.bass", Volume.Value, 0.5f);
        if (QueueWhenNoTargetsFound.Enabled) {
            // Queue for a game
            queueForGame = true;
            lastQueueTime = Misc::GetCurrentMs();
        };
    }

    lastHasTargets = hasTargets;
}

void AutoSnipe::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (packet->getId() == (int) PacketID::Text) {
        auto player = Minecraft::ClientInstance->ThePlayer();

        auto tp = std::reinterpret_pointer_cast<TextPacket>(packet);

        std::string text = tp->message.getText();

        if (text.find("§r§a§lVoting has ended!") != std::string::npos && QueueWhenNoTargetsFound.Enabled && !AnyTargetsFound()) {
            // Queue for a game
            queueForGame = true;
            lastQueueTime = Misc::GetCurrentMs();
        }

        if (text.find("joined. §8[12/12]") != std::string::npos && QueueWhenFull.Enabled && !AnyTargetsFound()) {
            // Queue for a game
            queueForGame = true;
            lastQueueTime = Misc::GetCurrentMs();
        }

        if (text.find("joined. §8[16/16]") != std::string::npos && QueueWhenFull.Enabled && !AnyTargetsFound()) {
            // Queue for a game
            queueForGame = true;
            lastQueueTime = Misc::GetCurrentMs();
        }

        if (text.find("§cYou are already connected to this server!") != std::string::npos && RetryQueue.Enabled) {
            // Queue for a game
            queueForGame = true;
            lastQueueTime = Misc::GetCurrentMs();
        }



    }
}

void AutoSnipe::OnEnable() {

}

void AutoSnipe::OnInitialize() {
    // Set the targets to the targets in preferences
    Targets = Prax::Preferences->AutoSnipeTargets;
}


