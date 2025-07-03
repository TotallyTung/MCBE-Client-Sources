//
// 8/22/2024.
//

#include "AutoKick.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PingUpdateEvent.hpp>
#include <Features/Events/SendImmediateEvent.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/World/BlockSource.hpp>
#include <SDK/Minecraft/World/BlockLegacy.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthinputPacket.hpp>
#include <SDK/Minecraft/Network/Packets/UpdateBlockPacket.hpp>
#include <SDK/Minecraft/Network/Packets/LevelEventPacket.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>

std::vector<glm::ivec3> getCollidingBlocks(Actor* target)
{
    std::vector<glm::ivec3> collidingBlockList;
    auto player = target;
    if (!player) return collidingBlockList;
    AABBShapeComponent* aabb = player->getAABBShapeComponent();
    glm::vec3 lower = aabb->mMin;
    glm::vec3 upper = aabb->mMin;

    lower.x -= 0.3f;
    //lower.y -= 0.1f;
    lower.z -= 0.3f;

    upper.x += 0.3f;
    upper.y += aabb->mHeight;
    upper.z += 0.3f;

    for (int x = floor(lower.x); x <= floor(upper.x); x++)
        for (int y = floor(lower.y); y <= floor(upper.y); y++)
            for (int z = floor(lower.z); z <= floor(upper.z); z++) {
                glm::ivec3 blockPos = { x, y, z };
                if (!BlockUtils::isAirBlock(blockPos)) {
                    collidingBlockList.emplace_back(blockPos);
                }
            }

    return collidingBlockList;
}

void AutoKick::reset() 
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    GameMode* gm = player->getGameMode();

    if (mIsMiningBlock) {
        gm->stopDestroyBlock(mCurrentBlockPos);
    }
    mCurrentBlockPos = { INT_MAX, INT_MAX, INT_MAX };
    mCurrentBlockFace = -1;
    mBreakingProgress = 0.f;
    mShouldSpoofSlot = true;
    mIsMiningBlock = false;
    mToolSlot = -1;
    mWasPlacingBlock = false;
}

bool AutoKick::isValidBlock(glm::ivec3 blockPos)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    Block* block = ClientInstance::get()->getBlockSource()->getBlock(blockPos);
    if (!block) return false;

    // Air Check
    if (block->mLegacy->isAir()) return false;

    // Distance Check
    AABB blockAABB = AABB(blockPos, glm::vec3(1, 1, 1));
    glm::vec3 closestPos = blockAABB.getClosestPoint(*player->getPos());
    if (7.f < glm::distance(closestPos, *player->getPos())) return false;

    // Exposed Check
    int exposedFace = BlockUtils::getExposedFace(blockPos);
    if (exposedFace == -1) return false;

    return true;
}

void AutoKick::queueBlock(glm::ivec3 blockPos)
{
    Block* block = ClientInstance::get()->getBlockSource()->getBlock(blockPos);
    mCurrentBlockPos = blockPos;
    mCurrentBlockFace = BlockUtils::getExposedFace(blockPos);
    if (mCurrentBlockFace == -1) mCurrentBlockFace = 0;
    mIsMiningBlock = true;
    mBreakingProgress = 0.f;
    int bestToolSlot = ItemUtils::getBestBreakingTool(block, mHotbarOnly.mValue);
    float destroySpeed = ItemUtils::getDestroySpeed(bestToolSlot, block);
    if (mDestroySpeed.mValue <= destroySpeed) mShouldRotateToBlock = true;
    PacketUtils::spoofSlot(bestToolSlot);
    mShouldSpoofSlot = false;
    BlockUtils::startDestroyBlock(blockPos, mCurrentBlockFace);
    mToolSlot = bestToolSlot;
    mShouldSetbackSlot = true;
}

void AutoKick::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AutoKick::onBaseTickEvent, nes::event_priority::VERY_LAST>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &AutoKick::onPacketOutEvent, nes::event_priority::VERY_LAST>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &AutoKick::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<SendImmediateEvent, &AutoKick::onSendImmediateEvent, nes::event_priority::VERY_LAST>(this);
    gFeatureManager->mDispatcher->listen<PingUpdateEvent, &AutoKick::onPingUpdateEvent, nes::event_priority::VERY_FIRST>(this);

    mLastTargetPos = { INT_MAX, INT_MAX, INT_MAX };
    mUsePrediction = false;
    mOnGroundTicks = 0;
}

void AutoKick::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AutoKick::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &AutoKick::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &AutoKick::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<SendImmediateEvent, &AutoKick::onSendImmediateEvent>(this);
    gFeatureManager->mDispatcher->deafen<PingUpdateEvent, &AutoKick::onPingUpdateEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;
    auto supplies = player->getSupplies();
    if (mSelectedSlot)
    {
        supplies->mSelectedSlot = mPreviousSlot;
        mSelectedSlot = false;
    }
    mShouldRotate = false;
    mRecentlyUpdatedBlockPositions.clear();
    mLastServerBlockPos = { INT_MAX,INT_MAX,INT_MAX };
}

void AutoKick::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    auto supplies = player->getSupplies();
    if (mMode.mValue == Mode::Push) {
        Actor* target = nullptr;
        auto actors = ActorUtils::getActorList(false, true);
        std::ranges::sort(actors, [&](Actor* a, Actor* b) -> bool
            {
                return a->distanceTo(player) < b->distanceTo(player);
            });

        for (auto actor : actors) {
            if (actor == player) continue;
            if (actor->distanceTo(player) > mRange.mValue) continue;

            target = actor;
        }


        // Ghost checks
        for (auto pos : mRecentlyUpdatedBlockPositions) {
            if (BlockUtils::isAirBlock(pos)) {
                if (mDebug.mValue) ChatUtils::displayClientMessage("Failed to place block");
                if (mSpamPlace.mValue) mLastBlockPlace = 0;
            }
            else {
                if (mDebug.mValue) ChatUtils::displayClientMessage("Successfully placed block");
                mLastBlockUpdated = NOW;
                mLastServerBlockPos = pos;
            }
        }
        mRecentlyUpdatedBlockPositions.clear();

        //ChatUtils::displayClientMessage("Ping:" + std::to_string(mPing));
        if (mLastBlockPlace + mDelay.mValue > NOW) {
            if (mSelectedSlot) {
                supplies->mSelectedSlot = mPreviousSlot;
                mSelectedSlot = false;
            }
            mLastTargetPos = { INT_MAX, INT_MAX, INT_MAX };
            mUsePrediction = false;
            return;
        }

        if (target == nullptr || !target->getActorTypeComponent()) return;
        if (!target->isPlayer()) return;

        if (ItemUtils::getAllPlaceables(mHotbarOnly.mValue) < 1) return;

        // Flagged Check
        std::vector<glm::ivec3> collidingBlocks = getCollidingBlocks(target);
        if (std::find(collidingBlocks.begin(), collidingBlocks.end(), mLastServerBlockPos) != collidingBlocks.end() && NOW - mLastBlockUpdated <= 1000) {
            if (!mFlagged) {
                int flagCount = mFlagCounter[target->getRawName()];
                mFlagCounter[target->getRawName()] = flagCount + 1;
                if (mDebug.mValue) {
                    ChatUtils::displayClientMessage(target->getRawName() + " has been flagged (x" + std::to_string(flagCount + 1) + ")");
                }
            }
            mFlagged = true;
        }
        else {
            mFlagged = false;
        }

        glm::vec3 targetPos = *target->getPos();
        glm::vec3 prediction = { 0, 0, 0 };
        float PingTime = ((mPing / 50) * 2) + ((mOpponentPing.mValue / 50) * 2);
        float PredictedTime = PingTime + 1;
        if (mUsePrediction) {
            prediction = targetPos - mLastTargetPos;
        }
        if (!mAllowDiagonal.mValue) {
            if (abs(prediction.z) < abs(prediction.x)) prediction.z = 0;
            else prediction.x = 0;
        }
        glm::vec3 predictionPos = targetPos + glm::vec3(prediction.x * PingTime, 0, prediction.z * PingTime);
        glm::vec3 predictionBlockPos = targetPos + glm::vec3(prediction.x * PredictedTime, 0, prediction.z * PredictedTime);
        mUsePrediction = true;
        mLastTargetPos = targetPos;
        if (floor(predictionPos.x) == floor(predictionBlockPos.x) && floor(predictionPos.z) == floor(predictionBlockPos.z)) return;
        targetPos = predictionPos;

        if (mOnGroundOnly.mValue) {
            if (prediction.y != 0) {
                return;
                mOnGroundTicks = 0;
            }
            mOnGroundTicks++;
            if (mOnGroundTicks < mMaxOnGroundTicks.mValue) return;
        }

        bool found = false;
        for (int i = 0; i <= 3; i++) {
            glm::ivec3 pos = glm::ivec3(floor(targetPos.x), floor(targetPos.y), floor(targetPos.z)) - glm::ivec3(0, i, 0);
            if (!BlockUtils::isAirBlock(pos + glm::ivec3(0, -1, 0))) {
                targetPos = pos;
                found = true;
                break;
            }
        }
        if (!found) return;

        int side = 1;
        int blockSlot = ItemUtils::getPlaceableItemOnBlock(targetPos, mHotbarOnly.mValue);
        if (blockSlot == -1) return;
        //if (BlockUtils::isAirBlock(targetPos + glm::vec3(0, -1, 0))) return;

        mPreviousSlot = supplies->mSelectedSlot;
        supplies->mSelectedSlot = blockSlot;
        mSelectedSlot = true;

        mCurrentPlacePos = targetPos;
        mShouldRotate = true;
        BlockUtils::placeBlock(targetPos, side);
        BlockUtils::placeBlock(targetPos + glm::vec3(0, 1, 0), side);
        mLastBlockPlace = NOW;
    }else if (mMode.mValue == Mode::Replace) {
        mPreviousSlot = supplies->mSelectedSlot;
        if(player->isDestroying()) return;

        if (isValidBlock(mCurrentBlockPos)) {
            Block* currentBlock = ClientInstance::get()->getBlockSource()->getBlock(mCurrentBlockPos);
            int exposedFace = BlockUtils::getExposedFace(mCurrentBlockPos);
            int bestToolSlot = ItemUtils::getBestBreakingTool(currentBlock, mHotbarOnly.mValue);
            mToolSlot = bestToolSlot;
            if (mShouldSpoofSlot) {
                PacketUtils::spoofSlot(bestToolSlot);
                mShouldSpoofSlot = false;
                return;
            }

            float destroySpeed = ItemUtils::getDestroySpeed(bestToolSlot, currentBlock);
            mBreakingProgress += destroySpeed;

            if (mDestroySpeed.mValue <= mBreakingProgress) {
                mShouldRotateToBlock = true;
                supplies->mSelectedSlot = bestToolSlot;
                BlockUtils::destroyBlock(mCurrentBlockPos, exposedFace, mInfiniteDurability.mValue);
                if (mDebug.mValue) ChatUtils::displayClientMessage("Destroyed Block");
                supplies->mSelectedSlot = mPreviousSlot;
                mIsMiningBlock = false;
                
                // Spoof slot and place block
                /*glm::ivec3 hitPos = mCurrentBlockPos + glm::ivec3(0, -1, 0);
                if (0 < ItemUtils::getAllPlaceables(mHotbarOnly.mValue) && !BlockUtils::isAirBlock(hitPos)) {
                    int blockSlot = ItemUtils::getPlaceableItemOnBlock(mCurrentBlockPos, mHotbarOnly.mValue, false);
                    if (blockSlot == -1) return;

                    std::shared_ptr<MobEquipmentPacket> mep = PacketUtils::createMobEquipmentPacket(blockSlot);

                    auto transac = MinecraftPackets::createPacket<InventoryTransactionPacket>();

                    auto cit = std::make_unique<ItemUseInventoryTransaction>();
                    cit->mActionType = ItemUseInventoryTransaction::ActionType::Place;
                    int slot = blockSlot;
                    cit->mSlot = slot;
                    cit->mItemInHand = *player->getSupplies()->getContainer()->getItem(slot);
                    cit->mBlockPos = mCurrentBlockPos + glm::ivec3(0, -1, 0);
                    cit->mFace = 1;
                    cit->mTargetBlockRuntimeId = 0;
                    cit->mPlayerPos = *player->getPos();

                    glm::vec3 pPos = *player->getPos() - glm::vec3(0, PLAYER_HEIGHT, 0);
                    glm::vec3 clickPos = glm::vec3(0.5, 1, 0.5);;


                    cit->mClickPos = clickPos;

                    transac->mTransaction = std::move(cit);

                    PacketUtils::queueSend(mep, false);
                    PacketUtils::queueSend(transac, false);
                    if (mDebug.mValue) ChatUtils::displayClientMessage("Replaced Block");
                }*/
                mWasPlacingBlock = true;
                return;
            }
        }
        else {
            if (mWasPlacingBlock) {
                glm::ivec3 hitPos = mCurrentBlockPos + glm::ivec3(0, -1, 0);
                if (0 < ItemUtils::getAllPlaceables(mHotbarOnly.mValue) && !BlockUtils::isAirBlock(hitPos)) {
                    int blockSlot = ItemUtils::getPlaceableItemOnBlock(mCurrentBlockPos, mHotbarOnly.mValue, false);
                    if (blockSlot == -1) return;
                    supplies->mSelectedSlot = blockSlot;
                    PacketUtils::spoofSlot(blockSlot);
                    BlockUtils::placeBlock(mCurrentBlockPos, 1);
                    supplies->mSelectedSlot = mPreviousSlot;
                    if (mDebug.mValue) ChatUtils::displayClientMessage("Replaced Block");
                    reset();
                    return;
                }
            }
            reset();

            for (auto& pos : mMiningBlocks) {
                queueBlock(pos);
                if (mDebug.mValue) ChatUtils::displayClientMessage("Queued Block");
                mMiningBlocks.clear();
                return;
            }

            if (mShouldSetbackSlot) {
                PacketUtils::spoofSlot(mPreviousSlot);
                mShouldSetbackSlot = false;
            }
        }

        mMiningBlocks.clear();
    }
}

void AutoKick::onPacketOutEvent(PacketOutEvent& event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::InventoryTransaction)
    {
        if (const auto it = event.getPacket<InventoryTransactionPacket>(); it->mTransaction->type ==
            ComplexInventoryTransaction::Type::ItemUseTransaction)
        {
            const auto transac = reinterpret_cast<ItemUseInventoryTransaction*>(it->mTransaction.get());
            if (transac->mActionType == ItemUseInventoryTransaction::ActionType::Place)
            {
                transac->mClickPos = BlockUtils::clickPosOffsets[transac->mFace];
                for (int i = 0; i < 3; i++)
                {
                    if (transac->mClickPos[i] == 0.5)
                    {
                        transac->mClickPos[i] = MathUtils::randomFloat(-0.49f, 0.49f);
                    }
                }
            }
        }
    }


    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();
        if (mShouldRotate)
        {
            const glm::vec3 blockPos = mCurrentPlacePos;
            auto blockAABB = AABB(blockPos, glm::vec3(1, 1, 1));
            glm::vec2 rotations = MathUtils::getRots(*player->getPos(), blockAABB);
            paip->mRot = rotations;
            paip->mYHeadRot = rotations.y;
            mShouldRotate = false;
        }
        if (mShouldRotateToBlock) {
            const glm::vec3 blockPos = mCurrentBlockPos;
            auto blockAABB = AABB(blockPos, glm::vec3(1, 1, 1));
            glm::vec2 rotations = MathUtils::getRots(*player->getPos(), blockAABB);
            paip->mRot = rotations;
            paip->mYHeadRot = rotations.y;
            mShouldRotateToBlock = false;
        }
    }
}

void AutoKick::onPacketInEvent(PacketInEvent& event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (mMode.mValue == Mode::Push) {
        if (event.mPacket->getId() == PacketID::UpdateBlock) {
            auto pkt = event.getPacket<UpdateBlockPacket>();
            auto updateBlockPacket = pkt.get();
            if (updateBlockPacket->mPos == mCurrentPlacePos && !BlockUtils::isAirBlock(updateBlockPacket->mPos) && NOW - mLastBlockPlace <= 800) {
                mRecentlyUpdatedBlockPositions.push_back(updateBlockPacket->mPos);
            }
        }
        else if (event.mPacket->getId() == PacketID::ChangeDimension) {
            mFlagCounter.clear();
        }
    }
    else if (mMode.mValue == Mode::Replace) {
        if (event.mPacket->getId() == PacketID::LevelEvent) {
            auto levelEvent = event.getPacket<LevelEventPacket>();
            if (levelEvent->mEventId == 3600) {
                if (isValidBlock(levelEvent->mPos) && !BlockUtils::isMiningPosition(levelEvent->mPos)) {
                    mMiningBlocks.push_back(levelEvent->mPos);
                }
            }
        }
    }
}

void AutoKick::onSendImmediateEvent(SendImmediateEvent& event)
{
    uint8_t packetId = event.send[0];
    if (packetId == 0)
    {
        uint64_t timestamp = *reinterpret_cast<uint64_t*>(&event.send[1]);
        uint64_t timestamp64 = _byteswap_uint64(timestamp);
        uint64_t now = NOW;
        mEventDelay = now - timestamp64;
    }
}

void AutoKick::onPingUpdateEvent(PingUpdateEvent& event)
{
    mPing = event.mPing - mEventDelay;
}