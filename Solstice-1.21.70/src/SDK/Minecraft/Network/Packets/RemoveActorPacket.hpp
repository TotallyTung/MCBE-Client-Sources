//
// Created by vastrakai on 7/19/2024.
//

#pragma once

#include "Packet.hpp"

class RemoveActorPacket : public Packet {
public:
    static const PacketID ID = PacketID::RemoveActor;

    int64_t mRuntimeID;
};