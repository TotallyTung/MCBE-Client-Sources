//
// Created by vastrakai on 7/2/2024.
//
#pragma once

#include "Event.hpp"
#include <memory>

class PacketInEvent : public CancelableEvent {
public:
    explicit PacketInEvent(std::shared_ptr<class Packet> packet, void* networkIdentifier, void* netEventCallback) : mPacket(packet), mNetworkIdentifier(networkIdentifier), mNetEventCallback(netEventCallback) {}

    std::shared_ptr<class Packet> mPacket;
    void* mNetworkIdentifier;
    void* mNetEventCallback;

    template <typename T>
    std::shared_ptr<T> getPacket() const
    {
        return std::reinterpret_pointer_cast<T>(mPacket);
    }
};