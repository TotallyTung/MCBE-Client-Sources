//
// Created by dark on 9/23/2024.
//

#include "FastMine.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/World/BlockSource.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>

void FastMine::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &FastMine::onBaseTickEvent>(this);

    if (mMode.mValue == Mode::BDS) {
        NotifyUtils::notify("Warning: Low destroy speed values may cause huge lag!", 3.f, Notification::Type::Warning);
    }
}

void FastMine::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &FastMine::onBaseTickEvent>(this);
}

void FastMine::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;

    glm::ivec3 blockPos = player->getLevel()->getHitResult()->mBlockPos;
    int side = player->getLevel()->getHitResult()->mFacing;
    Block* block = ClientInstance::get()->getBlockSource()->getBlock(blockPos);
    float breakProgress = player->getGameMode()->mBreakProgress;

    if (!block || !player->isDestroying()) return;

    if (mMode.mValue == Mode::BDS) {
        int count = ceilf(1 / mDestroySpeed.mValue) - 1;
        auto pap = MinecraftPackets::createPacket<PlayerAuthInputPacket>();
        pap->mClientTick = 0;
        pap->mInputMode = InputMode::Mouse;
        pap->mMove = { 0, 0 };
        pap->mPos = *player->getPos();
        pap->mPosDelta = { 0, 0, 0 };
        pap->mRot = { player->getActorRotationComponent()->mPitch, player->getActorRotationComponent()->mYaw };
        pap->mYHeadRot = player->getActorRotationComponent()->mYaw;
        for (int i = 0; i < count; i++) {
            ClientInstance::get()->getPacketSender()->sendToServer(pap.get());
        }
        return;
    }

    if (mDestroySpeed.mValue <= breakProgress) {
        BlockUtils::destroyBlock(blockPos, side, mMode.mValue == Mode::Hive && mInfiniteDurability.mValue);
    }
}
