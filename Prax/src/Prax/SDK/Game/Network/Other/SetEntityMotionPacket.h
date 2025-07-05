//
// Created by vastrakai on 9/15/2023.
//

#include <cstdint>
#include "src/Prax/SDK/Game/Network/Network.h"

class SetEntityMotionPacket : public Packet {
public:
    static const PacketID ID = PacketID::SetEntityMotion;
    int64_t RuntimeID;
    Vector3 Motion;

    SetEntityMotionPacket() = default;
};