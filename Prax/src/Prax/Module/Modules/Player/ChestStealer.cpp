//
// Created by vastrakai on 10/7/2023.
//

#include <Prax/UI/Notifications.h>
#include "ChestStealer.h"

#include <magic_enum.hpp>
#include <Prax/Hook/Hooks/RakPeerHooks.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/SDK/Game/Network/Other/InventoryTransactionPacket.h>

#include "InventoryManager.h"
#include "Prax/SDK/Game/Network/Other/InventoryContentPacket.h"

bool ChestStealer::IsStealing = false;
uint64_t ChestStealer::LastItemTaken = 0;
bool ChestStealer::IsChestOpen = false;
std::vector<NetworkItemStackDescriptor> ChestStealer::ItemsToTake = {};
ContainerID ChestStealer::CurrentContainerId = ContainerID::Chest;
uint64_t ChestStealer::LastOpen = 0;

#define NOW Misc::GetCurrentMs()

void ChestStealer::OnContainerTick(ContainerScreenController *csc) const {
    if (Mode == Silent) return;
    static InventoryManager* inventoryManager = ModuleManager::GetModule<InventoryManager>();

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (ChestStealer::LastItemTaken + (uint64_t)Delay.Value < Misc::GetCurrentMs()) {
        bool isNetskipping = RakPeerHooks::Ticking;

        if (isNetskipping && IgnoreWhileNetskipping.Enabled)
        {
            csc->_tryExit();
            return;
        }

        std::vector<int> itemz = {};
        for (int i = 0; i < 54; i++) {
            ItemStack* stack = player->containerManagerModel->getSlot(i);
            if (IgnoreUselessItems.Enabled && inventoryManager->IsUselessItem(stack, i)) continue;
            if (stack && stack->item) itemz.push_back(i);
        }


        if (!itemz.empty()) {
            if (ChestStealer::LastItemTaken + 200 < Misc::GetCurrentMs()) {
                Notifications::Notify(Prax::ClientName, "Stealing!", NotificationType::Info, 1.0f + ((float)Delay.Value / 1000.0f * itemz.size()));
                IsStealing = true;
            }
            for (int i : itemz) {
                csc->handleAutoPlace("container_items", i);
                ChestStealer::LastItemTaken = Misc::GetCurrentMs();
                break;
            }
        } else {
            // if the last item taken was more than 200ms ago and less than 1000ms ago, try to exit
            if (ChestStealer::LastItemTaken + 200 < Misc::GetCurrentMs()) {
                csc->_tryExit();
                IsStealing = false;
            }
        }
    }
}

int getFirstEmptySlot() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    for (int i = 0; i < 36; i++) {
        ItemStack* stack = player->supplies->inventory->getItem(i);
        if (!stack->item) return i;
    }

    return -1;
}

void ChestStealer::Reset()
{
    auto player = Minecraft::ClientInstance->ThePlayer();


    if (Mode == Silent)
    {
        auto packet = MinecraftPackets::CreatePacket<ContainerClosePacket>();
        packet->mContainerId = CurrentContainerId;
        packet->mServerInitiatedClose = false;
        Minecraft::ClientInstance->packetSender->sendToServer(packet.get());

        player->addChatMessage("[Stealer] Reset", 0);
    }

    IsStealing = false;
    IsChestOpen = false;

}

void ChestStealer::TakeItem(int slot, NetworkItemStackDescriptor item)
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    int from = slot;
    int to = getFirstEmptySlot();
    Logger::Write(__FUNCTION__, "from: " + std::to_string(from) + " to: " + std::to_string(to), Logger::Debug);
    if (to == -1) return;


    ItemStack* item2 = player->supplies->inventory->getItem(to);
    ItemStack item1 = *item2;
    item1.item = item.baseItem->getRawItem();
    item1.amount = item.mStackSize;

    // im gonna do something stupid for now
    Logger::Write(__FUNCTION__, "itemptr " + String::ToHex((uintptr_t)item2->item), Logger::Debug);
    Logger::Write(__FUNCTION__, "vtable " + String::ToHex((uintptr_t)item2->item), Logger::Debug);


    InventoryAction action1 = InventoryAction(from, &item1, item2);
    InventoryAction action2 = InventoryAction(to, item2, &item1);


    action1.mSource.mType = InventorySourceType::ContainerInventory;
    action2.mSource.mType = InventorySourceType::ContainerInventory;
    action1.mSource.mContainerId = (int)ContainerID::Chest;
    action2.mSource.mContainerId = (int)ContainerID::Inventory;

    auto pkt = MinecraftPackets::CreatePacket<InventoryTransactionPacket>();

    auto cit = std::make_unique<ComplexInventoryTransaction>();

    cit->data.addAction(action1);
    cit->data.addAction(action2);

    pkt->mTransaction = std::move(cit);

    Minecraft::ClientInstance->packetSender->sendToServer(pkt.get());

}

bool ChestStealer::DoDelay() const
{
    // Return true if the delay isnt reached yet
    if (Delay.Value != 0)
    {
        LastItemTaken = NOW;
        return true;
    }

    return false;
}

void ChestStealer::OnTick() {
    if (Minecraft::ClientInstance->GetScreenName() == "hud_screen" && Mode != Silent) {
        IsStealing = false;
    }

    if (Mode != Silent) return;

    if (!IsChestOpen)
    {
        IsStealing = false;
        return;
    }

    IsStealing = true;

    auto player = Minecraft::ClientInstance->ThePlayer();



    int itemIndex = 0;
    std::map<int, NetworkItemStackDescriptor> items = {};

    for (NetworkItemStackDescriptor& item : ItemsToTake) {
        if (item.mStackSize != 0)
        {
            items[itemIndex] = item;
        }

        itemIndex++;
    }

    uint64_t predictedStealTime = (static_cast<uint64_t>(Delay.Value) * items.size()) + 500 + LastOpen;

    if (predictedStealTime < NOW)
    {
        player->addChatMessage("[Stealer] Timed out", 0);
        Reset();
        return;
    }

    // Delay
    if (LastItemTaken + static_cast<uint64_t>(Delay.Value) > NOW) return;


    player->addChatMessage("Items to take: " + std::to_string(items.size()), 0);

    for (auto& [slot, item] : items)
    {
        TakeItem(slot, item);
        player->addChatMessage("Took item from slot " + std::to_string(slot), 0);
        // Set the stackSize to 0 so we don't take it again
        item.mStackSize = 0;
        if (DoDelay()) return;
    }


    Reset();
}


void ChestStealer::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) {
    if (Mode == Silent) {
        if (packet->getId() == (int)PacketID::ContainerOpen)
        {
            auto cop = std::reinterpret_pointer_cast<ContainerOpenPacket>(packet);
            if (cop->mContainerId != ContainerID::Chest) return;

            Logger::Write("Stealer", "Opened container " + std::to_string((int)cop->mContainerId), Logger::Info);


            CurrentContainerId = cop->mContainerId;
            IsChestOpen = true;
            LastOpen = NOW;
            *cancel = true;

        } else if (packet->getId() == (int)PacketID::ContainerClose)
        {
            auto ccp = std::reinterpret_pointer_cast<ContainerClosePacket>(packet);
            if (ccp->mContainerId != ContainerID::Chest) return;

            Logger::Write("Stealer", "Closed container " + std::to_string((int)ccp->mContainerId), Logger::Info);

            IsChestOpen = false;
            IsStealing = false;
            *cancel = true;

        } else if (packet->getId() == (int)PacketID::InventoryContent && IsChestOpen)
        {
            auto icp = std::reinterpret_pointer_cast<InventoryContentPacket>(packet);
            if (icp->mInventoryId == CurrentContainerId) {
                ItemsToTake = icp->mSlots;
                *cancel = true;
            }

            Logger::Write("Stealer", "InventoryContentPacket for container " + std::to_string((int)icp->mInventoryId), Logger::Info);
        }
    }
}

void ChestStealer::OnDisable() {
    ChestStealer::LastItemTaken = 0;
    IsStealing = false;
    Reset();
}
