#pragma once
//
// Created by vastrakai on 9/15/2023.
//


#include <cstdint>
#include <Prax/Utils/System/Memory.h>
#include <Prax/SDK/Game/Entity/Inventory.h>
#include <Prax/SDK/Game/Network/Network.h>



class MobEquipmentPacket : public Packet {
public:
    static const PacketID ID = PacketID::MobEquipment;
    __int64 RuntimeId;
    BUILD_ACCESS(this, int, HotbarSlot, 0x98); // 0x98 == 152
    BUILD_ACCESS(this, int, InventorySlot, 0x9C); // 0x9C == 156
    BUILD_ACCESS(this, int, WindowId, 0xA0); // 0xA0 == 160
    BUILD_ACCESS(this, int8_t, HotbarSlot2, 0xA1); // 0xA1 == 161
    BUILD_ACCESS(this, int8_t, InventorySlot2, 0xA2); // 0xA2 == 162
    BUILD_ACCESS(this, int8_t, WindowId2, 0xA3); // 0xA3 == 163

    static void Create(MobEquipmentPacket* _this, uint64_t runtimeId, ItemStack itemStack, int hotbarSlot, int inventorySlot) {
        using MobEquipmentPacketConstructor = void(__fastcall*)(MobEquipmentPacket*, uint64_t, ItemStack const&, int, int, void*);
        static auto _MobEquipmentPacketConstructor = (MobEquipmentPacketConstructor)(Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::MobEquipmentPacket_MobEquipmentPacket)));
        _MobEquipmentPacketConstructor(_this, runtimeId, itemStack, hotbarSlot, inventorySlot, nullptr);
    }

    MobEquipmentPacket() : Packet() {};

    MobEquipmentPacket(uint64_t runtimeId, ItemStack itemStack, int hotbarSlot, int inventorySlot) : Packet() {
        this->RuntimeId = runtimeId;
        this->HotbarSlot = hotbarSlot;
        this->InventorySlot = inventorySlot;
        this->WindowId = 0;
    }
};