#pragma once

class InteractPacket : public Packet {
public:
    // InteractPacket inner types define
    enum class Action : unsigned char {
        Invalid        = 0x0,
        RightClick     = 0x1,
        LeftClick      = 0x2,
        StopRiding     = 0x3, // Exit/Leave Vehicle
        InteractUpdate = 0x4, // MouseOver
        NpcOpen        = 0x5,
        OpenInventory  = 0x6,
    };
public:
    Action            mAction;   // this+0x30
    int               mTargetId; // this+0x38
    Vector3<float>    mPos;      // this+0x40
};