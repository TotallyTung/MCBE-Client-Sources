//
// 7/31/2024.
//

#include "Phase.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/RunUpdateCycleEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/World/Block.hpp>
#include <SDK/Minecraft/World/BlockLegacy.hpp>
#include <SDK/Minecraft/World/BlockSource.hpp>
#include <SDK/Minecraft/Network/Packets/MovePlayerPacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>

std::vector<glm::ivec3> Phase::getCollidingBlocks()
{
    std::vector<glm::ivec3> collidingBlockList;
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return collidingBlockList;


    AABBShapeComponent* aabb = player->getAABBShapeComponent();
    glm::vec3 playerPos = *player->getPos() - PLAYER_HEIGHT_VEC;
    glm::vec3 lower = playerPos;
    glm::vec3 upper = playerPos;
    float width = (aabb->mWidth / 2);

    lower.x -= width;
    lower.z -= width;

    upper.x += width;
    upper.y += aabb->mHeight;
    upper.z += width;

    for (int y = floor(lower.y); y <= floor(upper.y); y++)
        for (int x = floor(lower.x); x <= floor(upper.x); x++)
            for (int z = floor(lower.z); z <= floor(upper.z); z++) {
                glm::ivec3 blockPos = { x, y, z };
                if (!BlockUtils::isAirBlock(blockPos)) {
                    collidingBlockList.emplace_back(blockPos);
                }
            }

    return collidingBlockList;
}

void Phase::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Phase::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Phase::onPacketOutEvent, nes::event_priority::VERY_LAST>(this);
    gFeatureManager->mDispatcher->listen<RunUpdateCycleEvent, &Phase::onRunUpdateCycleEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;
    AABBShapeComponent* aabb = player->getAABBShapeComponent();
    if (mMode.mValue == Mode::Clip && !mTest.mValue && player->isOnGround()) {
        aabb->mMin.y -= 1;
        aabb->mMax.y -= 1;
    }
}

void Phase::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Phase::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Phase::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<RunUpdateCycleEvent, &Phase::onRunUpdateCycleEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;
    AABBShapeComponent* aabb = player->getAABBShapeComponent();
    if (aabb->mMax.y != aabb->mMin.y + aabb->mHeight) {
        aabb->mMax.y = aabb->mMin.y + aabb->mHeight;
    }

    if (mMode.mValue == Mode::Clip && !mTest.mValue && player->isOnGround()) {
        aabb->mMin.y += 1;
        aabb->mMax.y += 1;
    }

    mMoving = false;
    mClip = false;
}

void Phase::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    AABBShapeComponent* aabb = player->getAABBShapeComponent();

    if (mMode.mValue == Mode::Horizontal) {
        std::vector<glm::ivec3> collidingBlocks = getCollidingBlocks();
        mMoving = !collidingBlocks.empty();
        if (!mMoving) {
            mLastSync = NOW;
        }
        aabb->mMax.y = aabb->mMin.y;
    }
    else if (mMode.mValue == Mode::Vertical) {
        std::vector<glm::ivec3> collidingBlocks = getCollidingBlocks();

        auto moveInput = player->getMoveInputComponent();
        auto stateVector = player->getStateVectorComponent();
        bool isJumping = moveInput->mIsJumping;
        bool isSneaking = moveInput->mIsSneakDown;
        float value = 0;

        glm::vec3 belowBlockPos = *player->getPos();
        belowBlockPos.y -= (PLAYER_HEIGHT + 0.1f);
        glm::vec3 aboveBlockPos = *player->getPos();
        aboveBlockPos.y += 0.5f;

        if (isJumping && (!BlockUtils::isAirBlock(aboveBlockPos) || !collidingBlocks.empty())) {
            value = mSpeed.mValue / 10;
            aabb->mMin.y += value;
            aabb->mMax.y += value;
            stateVector->mVelocity = { 0, 0, 0 };
        }
        else if (isSneaking && (!BlockUtils::isAirBlock(belowBlockPos) || !collidingBlocks.empty())) {
            value = -(mSpeed.mValue / 10);
            aabb->mMin.y += value;
            aabb->mMax.y += value;
            stateVector->mVelocity = { 0, 0, 0 };
            moveInput->mIsSneakDown = false;
        }
    }
    else if (mMode.mValue == Mode::Clip) {
        if (mTest.mValue) {
            mClip = player->isOnGround();
        }else aabb->mMax.y = aabb->mMin.y;
    }
}

void Phase::onPacketOutEvent(PacketOutEvent& event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player || mMode.mValue != Mode::Clip || !mClip) return;

    if (event.mPacket->getId() == PacketID::PlayerAuthInput) {
        auto pkt = event.getPacket<PlayerAuthInputPacket>();
        pkt->mPos.y -= 1;
    }
}

void Phase::onRunUpdateCycleEvent(RunUpdateCycleEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (mMoving && mBlink.mValue) {
        if (mDelay.mValue) {
            if (mLastSync + mDelayMs.mValue > NOW) {
                event.mApplied = true;
                event.mCancelled = true;
            }
            else {
                mLastSync = NOW;
            }
        }
        else {
            event.mApplied = true;
            event.mCancelled = true;
        }
    }
}