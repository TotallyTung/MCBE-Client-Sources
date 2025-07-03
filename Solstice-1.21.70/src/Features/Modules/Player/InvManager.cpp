//
// Created by vastrakai on 7/6/2024.
//

#include "InvManager.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Events/PingUpdateEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/Packets/InteractPacket.hpp>
#include <SDK/Minecraft/Network/Packets/ContainerClosePacket.hpp>
#include <SDK/Minecraft/World/BlockLegacy.hpp>

void InvManager::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &InvManager::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &InvManager::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &InvManager::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->listen<PingUpdateEvent, &InvManager::onPingUpdateEvent>(this);
}

void InvManager::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &InvManager::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &InvManager::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &InvManager::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<PingUpdateEvent, &InvManager::onPingUpdateEvent>(this);
}

void InvManager::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    auto armorContainer = player->getArmorContainer();
    auto supplies = player->getSupplies();
    auto container = supplies->getContainer();

    if (mManagementMode.mValue != ManagementMode::Always && !mHasOpenContainer)
    {
        return;
    }

    // Check how many free slots we have
    int freeSlots = 0;
    for (int i = 0; i < 36; i++)
    {
        if (!container->getItem(i)->mItem) freeSlots++;
    }


    // If we are in a container, don't do anything
    if (ClientInstance::get()->getMouseGrabbed() && player && freeSlots > 0 && mManagementMode.mValue == ManagementMode::Always)
    {
        return;
    }

    std::vector<int> itemsToEquip;
    bool isInstant = mMode.mValue == Mode::Instant;
    if (mLastAction + static_cast<uint64_t>(mDelay.mValue) > NOW)
    {
        return;
    }

    int bestHelmetSlot = -1;
    int bestChestplateSlot = -1;
    int bestLeggingsSlot = -1;
    int bestBootsSlot = -1;
    int bestSwordSlot = -1;
    int bestPickaxeSlot = -1;
    int bestAxeSlot = -1;
    int bestShovelSlot = -1;

    int bestHelmetValue = 0;
    int bestChestplateValue = 0;
    int bestLeggingsValue = 0;
    int bestBootsValue = 0;
    int bestSwordValue = 0;
    int bestPickaxeValue = 0;
    int bestAxeValue = 0;
    int bestShovelValue = 0;

    int equippedHelmetValue = ItemUtils::getItemValue(armorContainer->getItem(0));
    int equippedChestplateValue = ItemUtils::getItemValue(armorContainer->getItem(1));
    int equippedLeggingsValue = ItemUtils::getItemValue(armorContainer->getItem(2));
    int equippedBootsValue = ItemUtils::getItemValue(armorContainer->getItem(3));

    int firstBowSlot = -1;
    int fireSwordSlot = ItemUtils::getFireSword(false);

    for (int i = 0; i < 36; i++)
    {
        auto item = container->getItem(i);
        if (!item->mItem) continue;
        auto itemType = item->getItem()->getItemType();

        if (item->getItem()->mName.contains("bow") && firstBowSlot == -1 && mDropExtraBows.mValue)
        {
            firstBowSlot = i;
        } else if (firstBowSlot != -1 && mDropExtraBows.mValue && item->getItem()->mName.contains("bow"))
        {
            supplies->getContainer()->dropSlot(i);

            mLastAction = NOW;
            if (!isInstant)
            {
                return;
            }
        }

        // This is so that we only ignore the first fire sword we find
        if (mIgnoreFireSword.mValue && fireSwordSlot != -1 && fireSwordSlot == i) continue;

        auto itemValue = ItemUtils::getItemValue(item);
        if (itemType == SItemType::Helmet && itemValue > bestHelmetValue)
        {
            if (equippedHelmetValue >= itemValue)
            {
                bestHelmetSlot = -1;
                continue;
            }

            bestHelmetSlot = i;
            bestHelmetValue = itemValue;
        }
        else if (itemType == SItemType::Chestplate && itemValue > bestChestplateValue)
        {
            if (equippedChestplateValue >= itemValue)
            {
                bestChestplateSlot = -1;
                continue;
            }

            bestChestplateSlot = i;
            bestChestplateValue = itemValue;
        }
        else if (itemType == SItemType::Leggings && itemValue > bestLeggingsValue)
        {
            if (equippedLeggingsValue >= itemValue)
            {
                bestLeggingsSlot = -1;
                continue;
            }

            bestLeggingsSlot = i;
            bestLeggingsValue = itemValue;
        }
        else if (itemType == SItemType::Boots && itemValue > bestBootsValue)
        {
            if (equippedBootsValue >= itemValue)
            {
                bestBootsSlot = -1;
                continue;
            }

            bestBootsSlot = i;
            bestBootsValue = itemValue;
        }
        else if (itemType == SItemType::Sword && itemValue > bestSwordValue)
        {
            bestSwordSlot = i;
            bestSwordValue = itemValue;
        }
        else if (itemType == SItemType::Pickaxe && itemValue > bestPickaxeValue)
        {
            bestPickaxeSlot = i;
            bestPickaxeValue = itemValue;
        }
        else if (itemType == SItemType::Axe && itemValue > bestAxeValue)
        {
            bestAxeSlot = i;
            bestAxeValue = itemValue;
        }
        else if (itemType == SItemType::Shovel && itemValue > bestShovelValue)
        {
            bestShovelSlot = i;
            bestShovelValue = itemValue;
        }
    }

    // Go through and get items to drop
    std::vector<int> itemsToDrop;
    for (int i = 0; i < 36; i++)
    {
        auto item = container->getItem(i);
        if (!item->mItem) continue;
        if (mIgnoreFireSword.mValue && fireSwordSlot != -1 && fireSwordSlot == i) continue;
        auto itemType = item->getItem()->getItemType();
        auto itemValue = ItemUtils::getItemValue(item);
        bool hasFireProtection = item->getEnchantValue(Enchant::FIRE_PROTECTION) > 0;

        if (mStealFireProtection.mValue && hasFireProtection) {
            continue;
        }

        if (itemType == SItemType::Sword && i != bestSwordSlot)
        {
            itemsToDrop.push_back(i);
        }
        else if (itemType == SItemType::Pickaxe && i != bestPickaxeSlot)
        {
            itemsToDrop.push_back(i);
        }
        else if (itemType == SItemType::Axe && i != bestAxeSlot)
        {
            itemsToDrop.push_back(i);
        }
        else if (itemType == SItemType::Shovel && i != bestShovelSlot)
        {
            itemsToDrop.push_back(i);
        }
        else if (itemType == SItemType::Helmet && i != bestHelmetSlot)
        {
            if (!(mStealFireProtection.mValue && hasFireProtection)) {
                itemsToDrop.push_back(i);
            }
        }
        else if (itemType == SItemType::Chestplate && i != bestChestplateSlot)
        {
            if (!(mStealFireProtection.mValue && hasFireProtection)) {
                itemsToDrop.push_back(i);
            }
        }
        else if (itemType == SItemType::Leggings && i != bestLeggingsSlot)
        {
            if (!(mStealFireProtection.mValue && hasFireProtection)) {
                itemsToDrop.push_back(i);
            }
        }
        else if (itemType == SItemType::Boots && i != bestBootsSlot)
        {
            if (!(mStealFireProtection.mValue && hasFireProtection)) {
                itemsToDrop.push_back(i);
            }
        }
    }

    for (auto& item : itemsToDrop)
    {
        supplies->getContainer()->dropSlot(item);

        mLastAction = NOW;
        if (!isInstant)
        {
            return;
        }
    }

    if (mPreferredSlots.mValue)
    {
        if (mPreferredSwordSlot.mValue != 0)
        {
            if (bestSwordSlot != -1 && bestSwordSlot != mPreferredSwordSlot.mValue - 1)
            {
                supplies->getContainer()->swapSlots(bestSwordSlot, mPreferredSwordSlot.mValue - 1);

                mLastAction = NOW;
                if (!isInstant)
                {
                    return;
                }
            }
        }
        if (mPreferredPickaxeSlot.mValue != 0)
        {
            if (bestPickaxeSlot != -1 && bestPickaxeSlot != mPreferredPickaxeSlot.mValue - 1)
            {
                supplies->getContainer()->swapSlots(bestPickaxeSlot, mPreferredPickaxeSlot.mValue - 1);

                mLastAction = NOW;
                if (!isInstant)
                {
                    return;
                }
            }
        }
        if (mPreferredAxeSlot.mValue != 0)
        {
            if (bestAxeSlot != -1 && bestAxeSlot != mPreferredAxeSlot.mValue - 1)
            {
                supplies->getContainer()->swapSlots(bestAxeSlot, mPreferredAxeSlot.mValue - 1);

                mLastAction = NOW;
                if (!isInstant)
                {
                    return;
                }
            }
        }
        if (mPreferredShovelSlot.mValue != 0)
        {
            if (bestShovelSlot != -1 && bestShovelSlot != mPreferredShovelSlot.mValue - 1)
            {
                supplies->getContainer()->swapSlots(bestShovelSlot, mPreferredShovelSlot.mValue - 1);

                mLastAction = NOW;
                if (!isInstant)
                {
                    return;
                }
            }
        }
        if (mPreferredFireSwordSlot.mValue != 0)
        {
            if (fireSwordSlot != -1 && fireSwordSlot != mPreferredFireSwordSlot.mValue - 1 && bestSwordSlot != fireSwordSlot)
            {
                supplies->getContainer()->swapSlots(fireSwordSlot, mPreferredFireSwordSlot.mValue - 1);

                mLastAction = NOW;
                if (!isInstant)
                {
                    return;
                }
            }
        }
        if (mPreferredBlocksSlot.mValue != 0)
        {
            ItemStack* item = container->getItem(mPreferredBlocksSlot.mValue - 1);
            if (!ItemUtils::isUsableBlock(item))
            {
                int firstPlaceable = ItemUtils::getFirstPlaceable(false);

                if (firstPlaceable != -1)
                {
                    supplies->getContainer()->swapSlots(firstPlaceable, mPreferredBlocksSlot.mValue - 1);

                    mLastAction = NOW;
                    if (!isInstant)
                    {
                        return;
                    }
                }
            }
        }
    }

    if (bestHelmetSlot != -1) itemsToEquip.push_back(bestHelmetSlot);
    if (bestChestplateSlot != -1) itemsToEquip.push_back(bestChestplateSlot);
    if (bestLeggingsSlot != -1) itemsToEquip.push_back(bestLeggingsSlot);
    if (bestBootsSlot != -1) itemsToEquip.push_back(bestBootsSlot);

    for (auto& item : itemsToEquip)
    {
        supplies->getContainer()->equipArmor(item);
        mLastAction = NOW;
        if (!isInstant)
        {
            break;
        }
    }
}

void InvManager::onPacketInEvent(PacketInEvent& event)
{
    if (event.mPacket->getId() == PacketID::ContainerOpen)
    {
        auto packet = event.getPacket<ContainerOpenPacket>();
        if (mManagementMode.mValue == ManagementMode::ContainerOnly || mManagementMode.mValue == ManagementMode::InvOnly && packet->mType == ContainerType::Inventory)
        {
            mHasOpenContainer = true;
        }
    }
    if (event.mPacket->getId() == PacketID::ContainerClose)
    {
        mHasOpenContainer = false;
    }
}

void InvManager::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::ContainerClose)
    {
        mHasOpenContainer = false;
    }
    else if (event.mPacket->getId() == PacketID::ContainerOpen)
    {
        auto packet = event.getPacket<ContainerOpenPacket>();
        if (mManagementMode.mValue == ManagementMode::ContainerOnly || mManagementMode.mValue == ManagementMode::InvOnly && packet->mType == ContainerType::Inventory)
        {
            mHasOpenContainer = true;
        }
    }
}

void InvManager::onPingUpdateEvent(PingUpdateEvent& event)
{
    mLastPing = event.mPing;
}

bool InvManager::isItemUseless(ItemStack* item, int slot)
{
    if (!item->mItem) return true;
    auto player = ClientInstance::get()->getLocalPlayer();
    SItemType itemType = item->getItem()->getItemType();
    auto itemValue = ItemUtils::getItemValue(item);
    auto Inv_Manager = gFeatureManager->mModuleManager->getModule<InvManager>();

    if (itemType == SItemType::Helmet || itemType == SItemType::Chestplate || itemType == SItemType::Leggings || itemType == SItemType::Boots)
    {
        int equippedItemValue = ItemUtils::getItemValue(player->getArmorContainer()->getItem(static_cast<int>(itemType)));
        bool hasFireProtection = item->getEnchantValue(Enchant::FIRE_PROTECTION) > 0;

        if (Inv_Manager->mStealFireProtection.mValue && hasFireProtection) {
            return false;
        }

        return equippedItemValue >= itemValue;
    }

    if (itemType == SItemType::Sword || itemType == SItemType::Pickaxe || itemType == SItemType::Axe || itemType == SItemType::Shovel)
    {
        int bestSlot = ItemUtils::getBestItem(itemType);
        int bestValue = ItemUtils::getItemValue(player->getSupplies()->getContainer()->getItem(bestSlot));

        return bestValue >= itemValue && bestSlot != slot;
    }

    return false;
}