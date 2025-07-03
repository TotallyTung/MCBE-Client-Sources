#pragma once
#include <chrono>

class NetworkStackLatencyPacket : public Packet {
public:
    std::chrono::steady_clock::time_point createTime;
    bool fromServer;
};