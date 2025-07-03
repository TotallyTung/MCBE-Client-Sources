//
// 3/3/2025.
//

#include "AutoEat.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>

void AutoEat::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AutoEat::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &AutoEat::onPacketOutEvent>(this);
}

void AutoEat::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AutoEat::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &AutoEat::onPacketOutEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;
}

void AutoEat::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    auto supplies = player->getSupplies();

    auto holdingItem = supplies->getContainer()->getItem(supplies->mSelectedSlot);
    if (mIgnoreBoomBox.mValue && holdingItem->mItem && StringUtils::containsIgnoreCase(holdingItem->getItem()->mName, "boombox")) {
        return;
    }

    if (mLastEat + 800 > NOW && !mShouldEat) {
        return;
    }

    int bowSlot = -1;
    int arrowSlot = -1;
    for (int i = 0; i < 36; i++)
    {
        auto item = player->getSupplies()->getContainer()->getItem(i);
        if (!item->mItem) continue;
        if (item->getItem()->mName.contains("bow"))
        {
            if (mHotbarOnly.mValue && i > 8) continue;
            bowSlot = i;
        }
        if (item->getItem()->mName.contains("arrow"))
        {
            arrowSlot = i;
        }
        if (bowSlot != -1 && arrowSlot != -1) break;
    }

    if (bowSlot == -1 || arrowSlot == -1) return;

    supplies->getContainer()->startUsingItem(bowSlot);
    mLastEat = NOW;
    mShouldEat = false;
}

void AutoEat::onPacketOutEvent(PacketOutEvent& event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::MobEquipment) {
        mShouldEat = true;
    }
}