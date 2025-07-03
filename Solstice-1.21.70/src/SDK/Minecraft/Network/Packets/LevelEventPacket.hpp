//
// Created by vastrakai on 7/13/2024.
//

#pragma once

#include "Packet.hpp"


class LevelEventPacket : public Packet {
public:
    static const PacketID ID = PacketID::LevelEvent;

    static const int EVENT_START_RAIN = 3001;
    static const int EVENT_START_THUNDER = 3002;
    static const int EVENT_STOP_RAIN = 3003;
    static const int EVENT_STOP_THUNDER = 3004;

    static const int EVENT_START_BREAKING = 3600;
    static const int EVENT_STOP_BREAKING = 3601;

    int mEventId;   // this+0x30
    glm::vec3 mPos; // this+0x34
    int mData;      // this+0x40

    LevelEventPacket() : Packet() {}
};


