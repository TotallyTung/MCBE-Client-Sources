//
// Created by vastrakai on 8/20/2023.
//

#include <cstdint>
#include "src/Prax/SDK/Game/Network/Network.h"

enum class InteractAction
{
    RIGHT_CLICK = 1,
    LEFT_CLICK = 2,
    LEAVE_VEHICLE = 3,
    MOUSEOVER = 4
};

class InteractPacket : public ::Packet
{
public:
    static const PacketID ID = PacketID::Interact;

    // InteractPacket inner types define
    enum class Action : unsigned char {
        Invalid        = 0x0,
        StopRiding     = 0x3,
        InteractUpdate = 0x4,
        NpcOpen        = 0x5,
        OpenInventory  = 0x6,
    };

public:
    InteractPacket::Action mAction;   // this+0x30
    uint64_t               mTargetId; // this+0x38
    Vector3                mPos;      // this+0x40

    // prevent constructor by default
    InteractPacket& operator=(InteractPacket const&);
    InteractPacket(InteractPacket const&);
};