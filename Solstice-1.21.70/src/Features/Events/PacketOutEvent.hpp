//
// Created by vastrakai on 6/30/2024.
//

#pragma once

#include "Event.hpp"
#include <SDK/Minecraft/Network/Packets/Packet.hpp>

class PacketOutEvent : public CancelableEvent {
public:
    Packet* mPacket;

    explicit PacketOutEvent(Packet* packet) : CancelableEvent() {
        this->mPacket = packet;
    }

    template <typename T>
    T* getPacket() const
    {
        return reinterpret_cast<T*>(mPacket);
    }
};