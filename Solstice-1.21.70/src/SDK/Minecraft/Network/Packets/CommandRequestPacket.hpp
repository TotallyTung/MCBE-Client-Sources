//
// Created by vastrakai on 7/5/2024.
//

#pragma once

#include "Packet.hpp"
#include <SDK/Minecraft/mce.hpp>

// skidding from levilamina 💯💯💯💯💯

enum class CommandOriginType : signed char {
    Player                   = 0x0,
    CommandBlock             = 0x1,
    MinecartCommandBlock     = 0x2,
    DevConsole               = 0x3,
    Test                     = 0x4,
    AutomationPlayer         = 0x5,
    ClientAutomation         = 0x6,
    DedicatedServer          = 0x7,
    Entity                   = 0x8,
    Virtual                  = 0x9,
    GameArgument             = 0xA,
    EntityServer             = 0xB,
    Precompiled              = 0xC,
    GameDirectorEntityServer = 0xD,
    Scripting                = 0xE,
    ExecuteContext           = 0xF,
};

struct CommandOriginData {
public:
    CommandOriginType mType;
    mce::UUID         mUuid;
    std::string       mRequestId;
    int64_t           mPlayerId;
};

class CommandRequestPacket : public Packet {
public:
    static const PacketID ID = PacketID::CommandRequest;
    std::string       mCommand;        // this+0x30
    CommandOriginData mOrigin;         // this+0x50
    bool              mInternalSource; // this+0x90
};
