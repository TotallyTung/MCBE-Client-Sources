//
// Created by vastrakai on 8/20/2024.
//

#pragma once

#include "Packet.hpp"

class LoginPacket : public ::Packet {
public:
    int                                      mClientNetworkVersion; // this+0x30
    std::unique_ptr<class ConnectionRequest> mConnectionRequest;    // this+0x38
};