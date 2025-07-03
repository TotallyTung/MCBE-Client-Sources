//
// Created by vastrakai on 7/22/2024.
//

#pragma once

#include "Packet.hpp"

enum class GameType : int {
    Undefined    = -1,
    Survival     = 0,
    Creative     = 1,
    Adventure    = 2,
    Default      = 5,
    Spectator    = 6,
    WorldDefault = 0,
};

class SetPlayerGameTypePacket : public ::Packet {
public:
    static inline PacketID ID = PacketID::SetPlayerGameType;

    GameType mPlayerGameType; // this+0x30
};