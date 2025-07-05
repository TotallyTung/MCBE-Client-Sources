//
// Created by vastrakai on 4/19/2024.
//

#include "AntiCheatDetector.h"

uint64_t AntiCheatDetector::LastPingPacket = 0;
uint64_t AntiCheatDetector::LastCheck = 0;
AntiCheatDetector::AntiCheats AntiCheatDetector::DetectedAntiCheat = AntiCheatDetector::AntiCheats::None;

#define NOW Misc::GetCurrentMs()

void AntiCheatDetector::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();

    // Return if last check was within 4 seconds
    if (NOW - LastCheck < 1000) return;

    // return if the lastcheck was within 3 seconds but the ping wasnt
    if (NOW - LastCheck < 3000 && NOW - LastPingPacket > 3000) return;

    if (DetectedAntiCheat != AntiCheats::None) {
        LastCheck = NOW;
        return;
    }

    // If we haven't received a ping packet in 5 seconds and the last check was later than the last ping packet, flag FlareonV2

    // hasRecenlyPinged - determined by if the lastPing was less than 5 seconds ago and if the last check was more than 1 second ago
    bool hasRecentlyPinged = LastPingPacket > 0 && NOW - LastPingPacket < 5000 && NOW - LastCheck > 1000;
    bool hasCheckedAfterPing = LastCheck > LastPingPacket;

    if (!hasRecentlyPinged && hasCheckedAfterPing) {
        DetectedAntiCheat = AntiCheats::FlareonV2;
        player->addChatMessage("§8[§cF§8]&r ", "§cFlareon V2 §fwas detected to be running. The new Anti-Cheat is currently §aactive§f.", true, true);
        Minecraft::ClientInstance->playUI("random.orb", 1.0f, 1.0f);
    } else if (hasRecentlyPinged) {
        DetectedAntiCheat = AntiCheats::FlareonV1;
        if (!V2NotifyOnly.Enabled)
        {
            player->addChatMessage("§8[§cF§8]&r ", "§cFlareon V1 §fwas detected to be running. The new Anti-Cheat is currently §cinactive§f.", true, true);
            Minecraft::ClientInstance->playUI("random.orb", 1.0f, 0.75f);
        }
    } else {
        player->addChatMessage("§8[§cF§8]&r ", "&cUnable to detect Anti-Cheat!", true, true);
    }
}

void AntiCheatDetector::OnEnable() {
    LastCheck = NOW;
    DetectedAntiCheat = AntiCheats::None;
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (player && !V2NotifyOnly.Enabled) {
        player->addChatMessage("§8[§cF§8]&r ", "&7Recalculating...", true, true);
        // play note to indicate waiting
        Minecraft::ClientInstance->playUI("random.orb", 1.0f, 0.5f);
    }
}

void AntiCheatDetector::OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {

}

void AntiCheatDetector::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (packet->getId() == (int)PacketID::ChangeDimension) {
        // If the player changes dimension, reset the anti-cheat detection
        DetectedAntiCheat = AntiCheats::None;
        LastCheck = NOW;
        LastPingPacket = 0;
        if (auto player = Minecraft::ClientInstance->ThePlayer()) {
            if (!V2NotifyOnly.Enabled)
                player->addChatMessage("§8[§cF§8]&r ", "&7Recalculating...", true, true);
        }
    }

    if (packet->getId() == (int)PacketID::NetworkStackLatency) {
        LastPingPacket = NOW;
    }
}

