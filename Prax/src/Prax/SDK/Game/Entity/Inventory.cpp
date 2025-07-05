//
// Created by vastrakai on 10/13/2023.
//

#include <Prax/SDK/Game/Network/Other/ItemStackRequestPacket.h>

#include "Inventory.h"

#include <Prax/SDK/Game/Minecraft.h>
#include <Prax/SDK/Game/Network/Other/InventoryTransactionPacket.h>

void InventoryTransactionManager::addAction(const InventoryAction& action)
{
    static void* func = reinterpret_cast<void*>(SigManager::GetSignatureAddress(SignatureEnum::InventoryTransactionManager_addAction));
    using addAction = void(__thiscall*)(InventoryTransactionManager*, const InventoryAction&, bool);
    reinterpret_cast<addAction>(func)(this, action, false);

}

void ItemStack::fromDescriptor(NetworkItemStackDescriptor const& desc)
{
    static uint64_t func = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ItemStack_fromDescriptor));
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    //Memory::CallFastcall<void>(reinterpret_cast<void*>(func), this, desc, player->level->getBlockPalette(), true);
    using fromDescriptor = void(__thiscall*)(ItemStack*, NetworkItemStackDescriptor const&, BlockPalette*, bool);
    reinterpret_cast<fromDescriptor>(func)(this, desc, player->level->getBlockPalette(), true);

}

std::string Item::GetName() {
    return direct_access<std::string>(this, 0xD0);
}

bool Item::IsSword() {
    return GetName().find("_sword") != std::string::npos;

}

bool Item::IsArmor2() {
    return (GetName().find("_helmet") != std::string::npos ||
            GetName().find("_chestplate") != std::string::npos ||
            GetName().find("_leggings") != std::string::npos ||
            GetName().find("_boots") != std::string::npos);
}

int Item::GetArmorSlot() {
    if (GetName().find("_helmet") != std::string::npos)
        return 0;
    else if (GetName().find("_chestplate") != std::string::npos)
        return 1;
    else if (GetName().find("_leggings") != std::string::npos)
        return 2;
    else if (GetName().find("_boots") != std::string::npos)
        return 3;
    else
        return 4;
}

bool Item::IsHelmet() {
    return GetName().find("_helmet") != std::string::npos;
}

bool Item::IsChestplate() {
    return GetName().find("_chestplate") != std::string::npos;
}

bool Item::IsLeggings() {
    return GetName().find("_leggings") != std::string::npos;
}

bool Item::IsBoots() {
    return GetName().find("_boots") != std::string::npos;
}

bool Item::IsPickaxe() {
    return GetName().find("_pickaxe") != std::string::npos;
}

bool Item::IsAxe() {
    return GetName().find("_axe") != std::string::npos;
}

bool Item::IsShovel() {
    return GetName().find("_shovel") != std::string::npos;
}

bool Item::IsBoombox() {
    return GetName().find("boombox") != std::string::npos;
}

bool Item::IsBowOrArrow() {

    return IsBow() || IsArrow();
}

bool Item::IsBow() {
    return GetName().find("bow") != std::string::npos;
}

bool Item::IsArrow() {
    return GetName().find("arrow") != std::string::npos;
}

bool Item::IsArmor() {
    return IsHelmet() || IsChestplate() || IsLeggings() || IsBoots();
}

bool Item::IsTool() {
    return IsPickaxe() || IsAxe() || IsShovel() || IsSword();
}

void Inventory::dropSlot(int slot) {
    auto player = Minecraft::ClientInstance->ThePlayer();

    ItemStack* itemStack = player->supplies->inventory->getItem(slot);

    if (!itemStack->item) return;

    Item* item = *itemStack->item;

    static ItemStack blankStack = ItemStack();

    InventoryAction action = InventoryAction(slot, itemStack, &blankStack);
    InventoryAction action2 = InventoryAction(0, &blankStack, itemStack);
    action.mSource.mContainerId = (int)ContainerID::Inventory;
    action2.mSource.mContainerId = (int)ContainerID::Inventory;
    action.mSource.mType = InventorySourceType::ContainerInventory;
    action2.mSource.mType = InventorySourceType::WorldInteraction;

    auto pkt = MinecraftPackets::CreatePacket<InventoryTransactionPacket>();

    auto cit = std::make_unique<ComplexInventoryTransaction>();
    cit->data.addAction(action);
    cit->data.addAction(action2);

    pkt->mTransaction = std::move(cit);
    Minecraft::ClientInstance->packetSender->sendToServer(pkt.get());

    player->supplies->inventory->setItem(slot, blankStack);

}

void Inventory::swapSlots(int from, int to)
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    ItemStack* item1 = player->supplies->inventory->getItem(from);
    ItemStack* item2 = player->supplies->inventory->getItem(to);

    InventoryAction action1 = InventoryAction(from, item1, item2);
    InventoryAction action2 = InventoryAction(to, item2, item1);

    action1.mSource.mType = InventorySourceType::ContainerInventory;
    action2.mSource.mType = InventorySourceType::ContainerInventory;
    action1.mSource.mContainerId = (int)ContainerID::Inventory;
    action2.mSource.mContainerId = (int)ContainerID::Inventory;

    auto pkt = MinecraftPackets::CreatePacket<InventoryTransactionPacket>();

    auto cit = std::make_unique<ComplexInventoryTransaction>();

    cit->data.addAction(action1);
    cit->data.addAction(action2);

    player->supplies->inventory->setItem(from, *item2);
    player->supplies->inventory->setItem(to, *item1);

    pkt->mTransaction = std::move(cit);

    Minecraft::ClientInstance->packetSender->sendToServer(pkt.get());

}

void Inventory::equipArmor(int slot)
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    ItemStack* itemStack = player->supplies->inventory->getItem(slot);
    if (!itemStack->item)
    {
        Logger::Write(__FUNCTION__, "Invalid item in slot " + std::to_string(slot), Logger::Error);
        return;
    }
    Item* item = *itemStack->item;

    // Get the current item stack in the armor slot
    ItemStack* armorStack = player->getArmorContainer()->getItem(item->GetArmorSlot());

    InventoryAction action = InventoryAction(slot, itemStack, armorStack);
    action.mSource.mType = InventorySourceType::ContainerInventory;
    action.mSource.mContainerId = (int)ContainerID::Inventory;

    InventoryAction action2 = InventoryAction(item->GetArmorSlot(), armorStack, itemStack);
    action2.mSource.mType = InventorySourceType::ContainerInventory;
    action2.mSource.mContainerId = (int)ContainerID::Armor;

    auto pkt = MinecraftPackets::CreatePacket<InventoryTransactionPacket>();

    auto cit = std::make_unique<ComplexInventoryTransaction>();

    cit->data.addAction(action);
    cit->data.addAction(action2);

    //player->supplies->inventory->setItem(slot, ItemStack::EMPTY_ITEM);
    //player->armorContainer->setItem(item->GetArmorSlot(), *itemStack);

    pkt->mTransaction = std::move(cit);

    Minecraft::ClientInstance->packetSender->sendToServer(pkt.get());

}

void Inventory::startUsingItem(int slot) {
    auto player = Minecraft::ClientInstance->ThePlayer();

    auto pkt = MinecraftPackets::CreatePacket<InventoryTransactionPacket>();

    auto cit = std::make_unique<ItemUseInventoryTransaction>();
    cit->actionType = ItemUseInventoryTransaction::ActionType::Use;
    cit->slot = slot;
    cit->itemInHand = *player->supplies->inventory->getItem(slot);


    pkt->mTransaction = std::move(cit);

    Minecraft::ClientInstance->packetSender->sendToServer(pkt.get());
}

void Inventory::releaseUsingItem(int slot) {
    auto player = Minecraft::ClientInstance->ThePlayer();

    /*auto pkt = MinecraftPackets::CreatePacket<InventoryTransactionPacket>(PacketID::InventoryTransaction);

    auto cit = std::make_unique<ItemReleaseInventoryTransaction>();
    cit->actionType = ItemReleaseInventoryTransaction::ActionType::Release;
    cit->slot = slot;
    cit->itemInHand = *player->supplies->inventory->getItem(slot);

    pkt->mTransaction = std::move(cit);

    Minecraft::ClientInstance->packetSender->sendToServer(pkt.get());*/

    int oldSlot = player->supplies->selectedHotbarSlot;
    player->SelectSlot(slot);
    player->gamemode->releaseUsingItem();
    player->SelectSlot(oldSlot);

}