#pragma once
//
// Created by vastrakai on 9/15/2023.
//


#include <cstdint>
#include "Packet.h"
#include "../../Utils/MemoryUtil.h"
#include "../../World/Item/ItemStack.h"

class MobEquipmentPacket : public Packet {
public:
    static const PacketID ID = PacketID::MobEquipment;
    __int64 RuntimeId;
    CLASS_MEMBER(int, HotbarSlot, 0x98); // 0x98 == 152
    CLASS_MEMBER(int, InventorySlot, 0x9C); // 0x9C == 156
    CLASS_MEMBER(int, WindowId, 0xA0); // 0xA0 == 160
    CLASS_MEMBER(int8_t, HotbarSlot2, 0xA1); // 0xA1 == 161
    CLASS_MEMBER(int8_t, InventorySlot2, 0xA2); // 0xA2 == 162
    CLASS_MEMBER(int8_t, WindowId2, 0xA3); // 0xA3 == 163

    MobEquipmentPacket(uint64_t runtimeId, ItemStack* itemStack, int hotbarSlot, int inventorySlot) {
        memset(this, 0x0, sizeof(MobEquipmentPacket));
        using MobEquipmentPacketConstructor_t = void(__fastcall*)(MobEquipmentPacket*, uint64_t, ItemStack*, int, int, char);
        static  MobEquipmentPacketConstructor_t  MobEquipmentPacketConstructor = (MobEquipmentPacketConstructor_t)(Addresses::MobEquipmentPacket_MobEquipmentPacket);
        MobEquipmentPacketConstructor(this, runtimeId, itemStack, hotbarSlot, inventorySlot, 0);
    }

    MobEquipmentPacket() : Packet() {};

};