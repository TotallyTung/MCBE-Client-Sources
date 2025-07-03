//
// Created by dark on 1/26/2025.
//

#pragma once

#include "Packet.hpp"

enum class Action : int {
    NoAction = 0,
    Swing = 1,
    WakeUp = 3,
    CriticalHit = 4,
    MagicCriticalHit = 5,
    RowRight = 128,
    RowLeft = 129,
};

class AnimatePacket : public Packet {
public:
    static const PacketID ID = PacketID::Animate;

    int64_t mRuntimeID;
    Action mAction;
    float mData;
};

