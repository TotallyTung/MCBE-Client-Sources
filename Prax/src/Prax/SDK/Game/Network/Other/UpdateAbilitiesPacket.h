//
// Created by vastrakai on 1/20/2024.
//

#pragma once

#include <Prax/SDK/Game/Network/Network.h>
// SerializedAbilitiesData inner types declare
    // clang-format off
    struct SerializedLayer;
// clang-format on

// SerializedAbilitiesData inner types define
enum class SerializedAbilitiesLayer : short {
    CustomCache = 0x0,
    Base        = 0x1,
    Spectator   = 0x2,
    Commands    = 0x3,
    Editor      = 0x4,
};

/*
BUILD(0B0000_0001),
MINE(0B0000_0010),
DOORS_AND_SWITCHES(0B0000_0100),
OPEN_CONTAINERS(0B0000_1000),
ATTACK_PLAYERS(0B0001_0000),
ATTACK_MOBS(0B0010_0000),
OPERATOR_COMMANDS(0B0100_0000),
TELEPORT(0B1000_0000),

INVULNERABLE,
FLYING,
MAY_FLY,
INSTABUILD,
LIGHTNING,
FLY_SPEED,
WALK_SPEED,
MUTED,
WORLD_BUILDER,
NO_CLIP,
PRIVILEGED_BUILDER;*/

struct SerializedLayer {
public:
    SerializedAbilitiesLayer mSerializedLayer; // this+0x0
    unsigned int                     mAbilitiesSet;    // this+0x4
    unsigned int                     mAbilityValues;   // this+0x8
    float                    mFlySpeed;        // this+0xC
    float                    mWalkSpeed;       // this+0x10


};


class UpdateAbilitiesPacket : public Packet {
public:
    static const PacketID ID = PacketID::UpdateAbilitiesPacket;

    uint64_t mTargetPlayer;       // this+0x0
    uint8_t mCommandPermissions; // this+0x8
    uint8_t mPlayerPermissions;  // this+0x9
    std::vector<SerializedLayer> mLayers; // this+0x10

};
