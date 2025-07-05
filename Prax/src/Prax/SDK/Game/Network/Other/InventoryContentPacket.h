//
// Created by vastrakai on 3/17/2024.
//

#pragma once

// Inventorycontentpacket
#include <Prax/SDK/Game/Entity/ContainerManagerModel.h>
#include <Prax/SDK/Game/Network/Network.h>
#include "InventoryTransactionPacket.h"

class InventoryContentPacket : public Packet
{
public:
    static const PacketID ID = PacketID::InventoryContent;

    ContainerID                             mInventoryId; // this+0x30
    std::vector<NetworkItemStackDescriptor> mSlots;       // this+0x38
};

class ContainerClosePacket : public ::Packet
{
public:
    static const PacketID ID = PacketID::ContainerClose;

    ContainerID mContainerId;          // this+0x30
    bool        mServerInitiatedClose; // this+0x31
};

class ContainerOpenPacket : public ::Packet
{
public:
    static const PacketID ID = PacketID::ContainerOpen;

    ContainerID          mContainerId;    // this+0x30
    ContainerType        mType;           // this+0x31
    BlockPos             mPos;            // this+0x34
    int64_t              mEntityUniqueID; // this+0x40
};