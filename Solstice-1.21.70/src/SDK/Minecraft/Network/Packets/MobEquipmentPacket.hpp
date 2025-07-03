//
// Created by vastrakai on 7/5/2024.
//

#pragma once

#include <SDK/Minecraft/Network/Packets/Packet.hpp>
#include <SDK/Minecraft/Inventory/NetworkItemStackDescriptor.hpp>



class MobEquipmentPacket : public ::Packet {
public:
    enum class ContainerID : signed char {
        None           = -1,
        Inventory      = 0,
        First          = 1,
        Last           = 100,
        Offhand        = 119,
        Armor          = 120,
        SelectionSlots = 122,
        PlayerUIOnly   = 124
    };
    static const PacketID ID = PacketID::MobEquipment;

    uint64_t                   mRuntimeId;    // this+0x30
    NetworkItemStackDescriptor mItem;         // this+0x38
    int                        mSlot;         // this+0x98
    int                        mSelectedSlot; // this+0x9C
    ContainerID                mContainerId;  // this+0xA0

    unsigned char mSlotByte;         // this+0xA1
    unsigned char mSelectedSlotByte; // this+0xA2
    unsigned char mContainerIdByte;  // this+0xA3
};