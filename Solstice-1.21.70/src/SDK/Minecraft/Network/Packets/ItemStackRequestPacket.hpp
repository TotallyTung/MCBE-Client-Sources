//
// Created by vastrakai on 11/18/2024.
//

#pragma once

#include "InventoryTransactionPacket.hpp"
#include "Packet.hpp"
#include <SDK/Minecraft/Inventory/ItemStackNetIdVariant.hpp>


class ItemStackRequestSlotInfo {
public:
    ContainerEnumName containerEnumName;
    uint8_t slot;
    PAD(6)

    ItemStackNetIdVariant networkStackID;

    ItemStackRequestSlotInfo() {
        memset(this, 0, sizeof(ItemStackRequestSlotInfo));
    }

    ItemStackRequestSlotInfo(ContainerEnumName containerEnumName, uint8_t slot, ItemStackNetIdVariant networkStackID) {
        memset(this, 0, sizeof(ItemStackRequestSlotInfo));
        this->containerEnumName = containerEnumName;
        this->slot = slot;
        this->networkStackID = networkStackID;
    }
};

enum ItemStackRequestActionType : uint8_t {
    Take = 0x0,
    Place = 0x1,
    Swap = 0x2,
    Drop = 0x3,
    Destroy = 0x4,
    Consume = 0x5,
    Create = 0x6,
    PlaceInItemContainer = 0x7,
    TakeFromItemContainer = 0x8,
    ScreenLabTableCombine = 0x9,
    ScreenBeaconPayment = 0xA,
    ScreenHUDMineBlock = 0xB,
    CraftRecipe = 0xC,
    CraftRecipeAuto = 0xD,
    CraftCreative = 0xE,
    CraftRecipeOptional = 0xF,
    CraftRepairAndDisenchant = 0x10,
    CraftLoom = 0x11,
    CraftNonImplemented_DEPRECATEDASKTYLAING = 0x12,
    CraftResults_DEPRECATEDASKTYLAING = 0x13,
    OnRender = 0x14,
};

class ItemStackRequestAction {
public:
    uintptr_t vtable;
    ItemStackRequestActionType type;
    PAD(7);
};

class ItemStackRequestActionTransferBase : public ::ItemStackRequestAction {};

class ItemStackRequestActionSwap : public ItemStackRequestActionTransferBase {
private:
    uint64_t one;
public:
    ItemStackRequestSlotInfo slot1;
    ItemStackRequestSlotInfo slot2;

    ItemStackRequestActionSwap(const ItemStackRequestSlotInfo& s1, const ItemStackRequestSlotInfo& s2) {
        //static uintptr_t vt = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemStackRequestAction_swap), 3);
        memset(this, 0x0, sizeof(ItemStackRequestActionSwap));
        //this->vtable = vt;
        this->type = ItemStackRequestActionType::Swap;
        this->one = 1;
        this->slot1 = s1;
        this->slot2 = s2;
    }
};

class ItemStackRequestActionDrop : public ItemStackRequestActionTransferBase {
public:
    PAD(0x57);

    ItemStackRequestActionDrop(uint8_t amount, bool randomDropLocation, const ItemStackRequestSlotInfo& slotInfo) {
        memset(this, 0x0, sizeof(ItemStackRequestActionDrop));
        using dropAction_ctor = void(__fastcall*)(ItemStackRequestActionDrop*, uint8_t, bool, const ItemStackRequestSlotInfo&);
        //static dropAction_ctor ctor = reinterpret_cast<dropAction_ctor>(Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemStackRequestAction_drop)));
        //ctor(this, amount, randomDropLocation, slotInfo);
    }
};



struct ItemStackRequestData {
    uintptr_t vtable;
    uint32_t weirdCounterThingy;
    uint32_t zero;
    uintptr_t zeroes[3];
    uintptr_t thingy;
    std::vector<ItemStackRequestAction*> actions;

    ItemStackRequestData(std::vector<ItemStackRequestAction*> actions) {
        static uintptr_t vt = 0; //Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemStackRequestData), 3);
        this->vtable = vt;
        this->zero = 0;
        memset(this->zeroes, 0, 24);
        this->weirdCounterThingy = 0xFFFFFFFD;
        this->thingy = 0x00000000FFFFFFFF;
        this->actions = actions;
    }
};

class ItemStackRequestBatch {
public:
    std::vector<std::unique_ptr<ItemStackRequestData>> mRequests;
};

class ItemStackRequestPacket : public Packet {
public:
    static inline PacketID ID = PacketID::ItemStackRequest;

    std::unique_ptr<ItemStackRequestBatch> mRequestBatch;

};
