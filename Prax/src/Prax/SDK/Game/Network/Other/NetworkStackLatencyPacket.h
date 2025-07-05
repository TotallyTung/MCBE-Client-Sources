//
// Created by vastrakai on 10/19/2023.
//

#pragma once

#include "Prax/SDK/Game/Network/Network.h"

class NetworkStackLatencyPacket : public Packet {
public:
    static const PacketID ID = PacketID::NetworkStackLatency;

    uint64_t createTime;
    bool fromServer;
};