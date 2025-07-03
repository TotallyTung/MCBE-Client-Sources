//
// Created by vastrakai on 7/2/2024.
//

#pragma once

#include "Packet.hpp"


class SetActorMotionPacket : public Packet {
public:
    static const PacketID ID = PacketID::SetActorMotion;
    int64_t mRuntimeID;
    glm::vec3 mMotion;

    SetActorMotionPacket() = default;

};