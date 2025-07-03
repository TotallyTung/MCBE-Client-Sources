//
// Created by vastrakai on 7/12/2024.
//

#include "AutoTool.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/World/BlockSource.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <SDK/Minecraft/World/Level.hpp>

void AutoTool::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AutoTool::onBaseTickEvent>(this);
}

void AutoTool::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AutoTool::onBaseTickEvent>(this);
    if (mOldSlot != -1)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return;
        player->getSupplies()->mSelectedSlot = mOldSlot;
        mOldSlot = -1;
    }
}

void AutoTool::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;


    if (!player->isDestroying())
    {
        if (mOldSlot != -1)
        {
            player->getSupplies()->mSelectedSlot = mOldSlot;
            mOldSlot = -1;
        }
        return;
    }

    auto blockSource = ClientInstance::get()->getBlockSource();
    auto blockPos = player->getLevel()->getHitResult()->mBlockPos;

    auto block = blockSource->getBlock(blockPos);
    if (!block) return;

    int bestSlot = ItemUtils::getBestBreakingTool(block, mHotbarOnly.mValue);
    if (bestSlot == -1) return;

    if (mOldSlot == -1) mOldSlot = player->getSupplies()->mSelectedSlot;
    else if (mFakeSpoof.mValue) player->getSupplies()->mInHandSlot = mOldSlot;

    player->getSupplies()->mSelectedSlot = bestSlot;
}
