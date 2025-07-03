#pragma once

class AnimatePacket : public Packet {
public:
    // AnimatePacket inner types define
    enum class Action : int {
        NoAction         = 0x0,
        Swing            = 0x1,
        WakeUp           = 0x3,
        CriticalHit      = 0x4,
        MagicCriticalHit = 0x5,
        RowRight         = 0x80,
        RowLeft          = 0x81,
    };

    ActorRuntimeID        mRuntimeId; // this+0x30
    AnimatePacket::Action mAction;    // this+0x38
    float                 mData;      // this+0x3C
};