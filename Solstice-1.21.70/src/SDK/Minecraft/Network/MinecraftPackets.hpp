#pragma once
//
// Created by vastrakai on 7/2/2024.
//

#include <memory>
#include <SDK/Minecraft/Network/PacketID.hpp>

#include "Packets/Packet.hpp"


class MinecraftPackets {
public:
    static std::shared_ptr<Packet> createPacket(PacketID id);

    template <typename T>
    static std::shared_ptr<T> createPacket() {
        // throw error if the type doesn't have an ID
        static_assert(std::is_base_of_v<Packet, T>, "T must inherit from Packet, and have a static ID member");
        const PacketID id = T::ID;
        return std::reinterpret_pointer_cast<T>(createPacket(id));
    }
};
