//
// Created by alteik on 03/09/2024.
//

#include "AutoSnipe.hpp"
#include "AutoQueue.hpp"
#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Modules/Misc/AutoQueue.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/Packets/Packet.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>
#include <SDK/Minecraft/World/Level.hpp>

std::vector<std::string> AutoSnipe::Targets = {};

std::vector<std::string> getDaPlayerList() {
    auto player = ClientInstance::get()->getLocalPlayer();
    std::vector<std::string> playerNames;
    if(!player) return playerNames;

    std::unordered_map<mce::UUID, PlayerListEntry>* playerList = player->getLevel()->getPlayerList();

    for (auto& entry : *playerList | std::views::values)
    {
        playerNames.emplace_back(StringUtils::toLower(entry.mName));
    }

    return playerNames;
}

bool AutoSnipe::AnyTargetsFound() {
    auto player = ClientInstance::get()->getLocalPlayer();
    auto playerList = getDaPlayerList();

    for (auto const& player : playerList) {
        if (std::find(Targets.begin(), Targets.end(), StringUtils::toLower(player)) != Targets.end()) {
            return true;
        }
    }

    return false;
}

bool queueForGame = false;
uint64_t lastQueueTime = 0;

void AutoSnipe::onEnable() {
    gFeatureManager->mDispatcher->listen<PacketInEvent, &AutoSnipe::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AutoSnipe::onBaseTickEvent>(this);
}

void AutoSnipe::onDisable() {
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &AutoSnipe::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AutoSnipe::onBaseTickEvent>(this);
}

void AutoSnipe::onBaseTickEvent(class BaseTickEvent &event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if(!player) return;

    if(Targets.empty()) return;

    static std::vector<std::string> oldPlayerList;
    auto playerList = getDaPlayerList();

    if (queueForGame && NOW - lastQueueTime > 1000) {
        if (!AnyTargetsFound()) {
            auto autoQueue = gFeatureManager->mModuleManager->getModule<AutoQueue>();
            if (autoQueue == nullptr)
            {
                ChatUtils::displayClientMessage("[AutoSnipe] ERROR: AutoQueue module not found.");
                return;
            }

            if (!autoQueue->mEnabled)
            {
                ChatUtils::displayClientMessage("[AutoSnipe] AutoQueue is not enabled!");
                return;
            }

            autoQueue->mQueueForGame = true;
            autoQueue->mLastQueueTime = NOW;
            queueForGame = false;
        } else {
            queueForGame = false;
            ChatUtils::displayClientMessage("[AutoSnipe] Queue cancelled! Targets found!");
        }
    }

    std::vector<std::string> newPlayers = {};
    std::vector<std::string> lostPlayers = {};

    for (auto const& player : playerList) {
        if (std::find(oldPlayerList.begin(), oldPlayerList.end(), player) == oldPlayerList.end()) {
            if (std::find(Targets.begin(), Targets.end(), player) == Targets.end()) {
                continue;
            }
            newPlayers.emplace_back(player);
        }
    }

    for (auto const& player : oldPlayerList) {
        if (std::find(playerList.begin(), playerList.end(), player) == playerList.end()) {
            if (std::find(Targets.begin(), Targets.end(), player) == Targets.end()) {
                continue;
            }
            lostPlayers.emplace_back(player);
        }
    }

    for (auto const& player : newPlayers) {
        ChatUtils::displayClientMessage("[AutoSnipe] Found target: " + player);
    }

    for (auto const& player : lostPlayers) {
        ChatUtils::displayClientMessage("[AutoSnipe] Lost target: " + player);
    }

    oldPlayerList = playerList;

    bool hasTargets = AnyTargetsFound();
    static bool lastHasTargets = false;

    if (!hasTargets && lastHasTargets) {
        ChatUtils::displayClientMessage("[AutoSnipe]  No more targets found!");

        if (mQueueWhenNoTargetsFound.mValue) {
            queueForGame = true;
            lastQueueTime = NOW;
        };
    }

    lastHasTargets = hasTargets;
}

void AutoSnipe::onPacketInEvent(class PacketInEvent &event) {
    if(Targets.empty()) return;

    if (event.mPacket->getId() == PacketID::Text) {
        auto tp = event.getPacket<TextPacket>();
        auto player = ClientInstance::get()->getLocalPlayer();
        if(!player) return;

        std::string text = tp->mMessage;

        if (text.find("§r§a§lVoting has ended!") != std::string::npos && mQueueWhenNoTargetsFound.mValue && !AnyTargetsFound()) {
            queueForGame = true;
            lastQueueTime = NOW;
            ChatUtils::displayClientMessage("[AutoSnipe] No targets found");
        }

        if (text.find("joined. §8[12/12]") != std::string::npos && mQueueWhenFull.mValue && !AnyTargetsFound()) {
            queueForGame = true;
            lastQueueTime = NOW;
            ChatUtils::displayClientMessage("[AutoSnipe] No targets found");
        }

        if (text.find("joined. §8[16/16]") != std::string::npos && mQueueWhenFull.mValue && !AnyTargetsFound()) {
            queueForGame = true;
            lastQueueTime = NOW;
            ChatUtils::displayClientMessage("[AutoSnipe] No targets found");
        }

        if (text.find("§cYou are already connected to this server!") != std::string::npos && mRetryQueue.mValue) {
            queueForGame = true;
            lastQueueTime = NOW;
            ChatUtils::displayClientMessage("[AutoSnipe] re-queue");
        }
    }
}