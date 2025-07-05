//
// Created by vastrakai on 8/12/2023.
//

#include <cstdint>
#include "src/Prax/SDK/Game/Misc/Structs.h"
#include "src/Prax/SDK/Game/Network/Network.h"

class MovePlayerPacket : public Packet {
public:
    static const PacketID ID = PacketID::MovePlayer;

    /* Fields */
    int64_t actorRuntimeID;
    Vector3 position;
    Vector2 rotation;
    float headYaw;
    uint8_t mode;
    bool onGround;
    uint64_t ridingRuntimeId;
    int teleportCause;
    __int32 teleportItem;
    __int32 tick;

    MovePlayerPacket() = default;

    MovePlayerPacket(const MovePlayerPacket& other)
    {
        memcpy(this, &other, sizeof(MovePlayerPacket));
    }
    void operator=(const MovePlayerPacket& other)
    {
        memcpy(this, &other, sizeof(MovePlayerPacket));
    }
};
