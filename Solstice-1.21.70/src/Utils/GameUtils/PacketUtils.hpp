#pragma once
#include <complex.h>
#include <complex.h>
#include <SDK/Minecraft/Network/Packets/MobEquipmentPacket.hpp>
#include <SDK/Minecraft/Network/Packets/Packet.hpp>
//
// Created by vastrakai on 7/5/2024.
//


class PacketUtils {
public:
    static void spoofSlot(int slot, bool bypassHook = true);
    static std::shared_ptr<MobEquipmentPacket> createMobEquipmentPacket(int slot);

    template <typename T>
    static void sendToSelf(const std::shared_ptr<T>& packet) {
        sendToSelf(std::reinterpret_pointer_cast<Packet>(packet));
    }
    static void sendToSelf(const std::shared_ptr<Packet>& packet);
    static void queueSend(std::shared_ptr<Packet> packet, bool bypassHook = true);
    static void sendChatMessage(const std::string& msg);
};