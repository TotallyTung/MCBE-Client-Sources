#include "Nuker.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/Network/PacketID.hpp>
#include <SDK/Minecraft/Network/Packets/InventoryTransactionPacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <SDK/Minecraft/Network/Packets/LevelEventPacket.hpp>
#include <SDK/Minecraft/Network/Packets/MobEquipmentPacket.hpp>
#include <SDK/Minecraft/World/BlockLegacy.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>

void Nuker::reset() {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    GameMode* gm = player->getGameMode();

    if (mIsMiningBlock) {
        gm->stopDestroyBlock(mCurrentBlockPos);
    }
    mCurrentBlockPos = { 0, 0, 0 };
    mCurrentBlockFace = -1;
    mBreakingProgress = 0.f;
    mShouldSpoofSlot = true;
    mIsMiningBlock = false;
    mToolSlot = -1;
}

bool Nuker::isValidBlock(glm::ivec3 blockPos) {
    auto player = ClientInstance::get()->getLocalPlayer();
    Block* block = ClientInstance::get()->getBlockSource()->getBlock(blockPos);
    if (!block) return false;

    //
    if (block->getmLegacy()->isAir()) return false;

    // Distance Check
    AABB blockAABB = AABB(blockPos, glm::vec3(1, 1, 1));
    glm::vec3 closestPos = blockAABB.getClosestPoint(*player->getPos());
    if (mRange.mValue < glm::distance(closestPos, *player->getPos())) return false;

    // Exposed check
    std::vector<Block*> surroundingBlocks;

    // Go through each side and get the block on it

    bool hasRedstone = false;

    for (int i = 0; i < offsetList.size(); i++) {
        glm::ivec3 offset = offsetList[i];
        glm::ivec3 newPos = blockPos + offset;
        Block* newBlock = ClientInstance::get()->getBlockSource()->getBlock(newPos);
        if (!newBlock) continue;
        if (newBlock->getmLegacy()->getBlockId() == 73 || newBlock->getmLegacy()->getBlockId() == 74)
        {
            hasRedstone = true;
        }
        if (newBlock->getmLegacy()->isAir()) {
            hasRedstone = false;
            break;
        }
    }

    if (hasRedstone) return false;

    return true;
}

void Nuker::queueBlock(glm::ivec3 blockPos) {
    Block* block = ClientInstance::get()->getBlockSource()->getBlock(blockPos);
    mCurrentBlockPos = blockPos;
    mCurrentBlockFace = BlockUtils::getExposedFace(blockPos);
    if (mCurrentBlockFace == -1) mCurrentBlockFace = 0;
    mIsMiningBlock = true;
    mBreakingProgress = 0.f;
    int bestToolSlot = ItemUtils::getBestBreakingTool(block, mHotbarOnly.mValue);
    if (mTest.mValue) bestToolSlot = ItemUtils::getBestItem(SItemType::Shovel, mHotbarOnly.mValue);
    if (mShouldSpoofSlot) {
        PacketUtils::spoofSlot(bestToolSlot);
        mShouldSpoofSlot = false;
    }
    BlockUtils::startDestroyBlock(blockPos, mCurrentBlockFace);
    if (mInstaBreak) BlockUtils::destroyBlock(blockPos, mCurrentBlockFace, mInfiniteDurability.mValue);
    mToolSlot = bestToolSlot;
    mShouldSetbackSlot = true;
}

void Nuker::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Nuker::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<RenderEvent, &Nuker::onRenderEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Nuker::onPacketOutEvent, nes::event_priority::VERY_LAST>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    mShouldRotate = false;
    mIsMiningBlock = false;
    reset();
}

void Nuker::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Nuker::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<RenderEvent, &Nuker::onRenderEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Nuker::onPacketOutEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (mIsMiningBlock) {
        player->getSupplies()->mSelectedSlot = mToolSlot;
        player->getGameMode()->stopDestroyBlock(mCurrentBlockPos);
    }
}

void Nuker::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    if(player->isDestroying()) reset();
    BlockSource* source = ClientInstance::get()->getBlockSource();
    if (!source) return;
    PlayerInventory* supplies = player->getSupplies();
    mPreviousSlot = supplies->getmSelectedSlot();

    // Return without reset breaking progress
    if (mLastBlockPlace + 100 > NOW) {
        if (mIsMiningBlock) PacketUtils::spoofSlot(mLastPlacedBlockSlot);
        return;
    }

    if (isValidBlock(mCurrentBlockPos)) { // Check if current block is valid
        Block* currentBlock = source->getBlock(mCurrentBlockPos);
        int exposedFace = BlockUtils::getExposedFace(mCurrentBlockPos);
        int bestToolSlot = ItemUtils::getBestBreakingTool(currentBlock, mHotbarOnly.mValue);
        if (mTest.mValue) bestToolSlot = ItemUtils::getBestItem(SItemType::Shovel, mHotbarOnly.mValue);
        if (mShouldSpoofSlot) {
            PacketUtils::spoofSlot(bestToolSlot);
            mShouldSpoofSlot = false;
        }
        mToolSlot = bestToolSlot;

        float destroySpeed = ItemUtils::getDestroySpeed(bestToolSlot, currentBlock);

        mBreakingProgress += destroySpeed;

        if (mDestroySpeed.mValue <= mBreakingProgress) {
            mShouldRotate = true;
            supplies->mSelectedSlot = bestToolSlot;
            if (mSwing.mValue) player->swing();
            BlockUtils::destroyBlock(mCurrentBlockPos, exposedFace, mInfiniteDurability.mValue);
            supplies->mSelectedSlot = mPreviousSlot;
            mIsMiningBlock = false;
            return;
        }
    }
    else { // Find new block
        reset();
        std::vector<BlockInfo> blockList = BlockUtils::getBlockList(*player->getPos(), mRange.mValue);

        for (int i = 0; i < blockList.size(); i++) {
            if (!isValidBlock(blockList[i].mPosition) || (mBlockType.mValue == BlockType::Specified && blockList[i].mBlock->getmLegacy()->getmName() != specifiedBlockID)) continue;
            queueBlock(blockList[i].mPosition);
            return;
        }

        if (mShouldSetbackSlot) {
            PacketUtils::spoofSlot(mPreviousSlot);
            mShouldSetbackSlot = false;
        }
    }
}


void Nuker::onRenderEvent(RenderEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (mRenderBlock.mValue && mIsMiningBlock)
    {
        static float lastProgress = 0.f;
        float progress = 1.f;

        progress = mBreakingProgress;
        progress /= mDestroySpeed.mValue;
        if (progress < lastProgress) lastProgress = progress;
        progress = MathUtils::lerp(lastProgress, progress, ImGui::GetIO().DeltaTime * 30.f);
        lastProgress = progress;

        // clamp the progress to 0-1
        progress = MathUtils::clamp(progress, 0.f, 1.f);

        if (progress < 0.01f) return;


        auto size = glm::vec3(progress, progress, progress);
        glm::vec3 blockPos = mCurrentBlockPos;
        blockPos.x += 0.5f - (progress / 2.f);
        blockPos.y += 0.5f - (progress / 2.f);
        blockPos.z += 0.5f - (progress / 2.f);
        auto blockAABB = AABB(blockPos, size);
        RenderUtils::drawOutlinedAABB(blockAABB, true, ColorUtils::getThemedColor(0));
    }

}

void Nuker::onPacketOutEvent(PacketOutEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();
        if (mShouldRotate)
        {
            const glm::vec3 blockPos = mCurrentBlockPos;
            auto blockAABB = AABB(blockPos, glm::vec3(1, 1, 1));
            glm::vec2 rotations = MathUtils::getRots(*player->getPos(), blockAABB);
            paip->mRot = rotations;
            paip->mYHeadRot = rotations.y;
            mShouldRotate = false;
        }
    }
    else if (event.mPacket->getId() == PacketID::InventoryTransaction) {
        if (const auto it = event.getPacket<InventoryTransactionPacket>();
            it->mTransaction->type == ComplexInventoryTransaction::Type::ItemUseTransaction)
        {
            if (const auto transac = reinterpret_cast<ItemUseInventoryTransaction*>(it->mTransaction.get());
                transac->mActionType == ItemUseInventoryTransaction::ActionType::Place)
            {
                mLastBlockPlace = NOW;
                mLastPlacedBlockSlot = transac->mSlot;
            }
        }
    }
    else if (event.mPacket->getId() == PacketID::MobEquipment) {
        auto mpkt = event.getPacket<MobEquipmentPacket>();
        if (mpkt->mSlot != mToolSlot) mShouldSpoofSlot = true;
    }
}