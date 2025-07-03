//
// Created by vastrakai on 10/12/2024.
//
#pragma once

#include <chrono>
#include "Packet.hpp"

class NetworkStackLatencyPacket : public ::Packet {
public:
    static inline PacketID ID = PacketID::NetworkStackLatency;
    uint64_t                              mCreateTime; // this+0x30
    bool                                  mFromServer; // this+0x38
};