//
// Created by vastrakai on 10/19/2023.
//

#include "Phase.h"

#include <Prax/Hook/Hooks/RakPeerHooks.h>
#include <Prax/SDK/Game/Internal/MinecraftGame.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/Utils/System/KeyManager.h>

float oldHeight = 0;

// Checks if the player is in a block
bool checkPlayerAabb()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    bool isInBlock = player->isInWall();
    bool stuckInCollider = player->IsStuckInCollider();


    return isInBlock || stuckInCollider;
}

static bool WasBlinking = false;

void Phase::OnEnable() {
    if (Mode.SelectedIndex != 0) return;

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    auto shapeComponent = player->GetAABBShapeComponent();
    if (!shapeComponent) return;
    oldHeight = shapeComponent->BBY - shapeComponent->AAY;
    shapeComponent->BBY = shapeComponent->AAY;
}

#ifndef __RELEASEFREE__

std::vector<BlockPos> getCollidingBlocks()
{
    auto closestActor = Minecraft::ClientInstance->ThePlayer();
    if (!closestActor) return {};

    AABB aabb = closestActor->GetAABB();

    // Get all blocks that the actor is colliding with
    std::vector<BlockPos> collidingBlocks = std::vector<BlockPos>();
    Vector3 max = aabb.upper;
    Vector3 min = aabb.lower;
    max.x += 0.1f;
    max.z += 0.1f;
    min.x -= 0.1f;
    min.z -= 0.1f;
    max.y += 1.f;
    min.y += 1.f;


    // Add some padding to the AABB

    // Add each corner of the AABB to the list of blocks (Round down to the nearest block)
    // Do NOT floor the y value
    collidingBlocks.emplace_back(BlockPos(floor(max.x), max.y, floor(max.z)));
    collidingBlocks.emplace_back(BlockPos(floor(max.x), max.y, floor(min.z)));
    collidingBlocks.emplace_back(BlockPos(floor(min.x), max.y, floor(max.z)));
    collidingBlocks.emplace_back(BlockPos(floor(min.x), max.y, floor(min.z)));

    // Add the blocks in between the corners
    for (int x = floor(min.x); x <= floor(max.x); x++) {
        for (int z = floor(min.z); z <= floor(max.z); z++) {
            collidingBlocks.emplace_back(BlockPos(x, max.y, z));
        }
    }

    // Add the blocks below the actor
    for (int x = floor(min.x); x <= floor(max.x); x++) {
        for (int z = floor(min.z); z <= floor(max.z); z++) {
            collidingBlocks.emplace_back(BlockPos(x, max.y - 1, z));
        }
    }

    return collidingBlocks;
}

bool checkBlock()
{
    // Check if the blocks around the bottom of AABB are air
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;

    auto collidingBlocks = getCollidingBlocks();

    for (auto blockPos : collidingBlocks) {
        if (!Misc::IsAirBlock(blockPos)) {
            return true;
        }
    }

    return false;
}

#endif

void tpToY() {

}

void Phase::OnTick()
{
    // Check canuseKeys
    if (!Minecraft::ClientInstance->mcGame->canUseKeys()) return;
    if (Mode.SelectedIndex == 0)
    {
#ifndef __RELEASEFREE__
        if (checkBlock() && Blink.Enabled)
        {
            RakPeerHooks::Ticking = true;
            RakPeerHooks::TicksToSkip = 100000; // Never send until we're done
            RakPeerHooks::MsToSkip = 0; // Never send until we're done
            RakPeerHooks::Visualize = true;
            if (!WasBlinking) {
                RakPeerHooks::LastPos = *Minecraft::ClientInstance->ThePlayer()->getPos();
            }
            WasBlinking = true;
        } else
        {
            if (WasBlinking)
            {
                RakPeerHooks::Ticking = false;
                RakPeerHooks::TicksToSkip = 0;
                RakPeerHooks::MsToSkip = 0;
                RakPeerHooks::LastPos = *Minecraft::ClientInstance->ThePlayer()->getPos();
                WasBlinking = false;
            }
        }
#endif
        return;
    }

    // Desync (using RakPeerHooks)


    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    bool aabbCheck = checkPlayerAabb();
    auto moveInputComponent = player->GetMoveInputComponent();

    bool isSneaking = KeyManager::Keys[VK_SHIFT];
    bool isJumping = KeyManager::Keys[VK_SPACE];

    Vector3 heightOffset = Vector3(0.f, player->GetHeightOffset(), 0.f);

    Vector3 pos = *player->getPos() - heightOffset;

    Vector3 blockPos = *player->getPos() - Vector3(0.f, 1.62 - 0.1, 0.f);
    blockPos.x = floor(blockPos.x);
    blockPos.y = floor(blockPos.y);
    blockPos.z = floor(blockPos.z);

    float speed = VerticalSpeed.Value / 10;

    /*if (aabbCheck && isJumping && !Misc::IsAirBlock(blockPos))
    {
        //player->teleportTo(pos + Vector3(0.f, speed, 0.f), false, 0, 0, false);
        player->GetAABBShapeComponent()->AAY += speed;
        player->GetAABBShapeComponent()->BBY += speed;
        player->GetStateVectorComponent()->velocity = Vector3(0.f, 0.f, 0.f);
        moveInputComponent->isJumping = false;
    }

    if (player->isOnGround() && isSneaking || aabbCheck && isSneaking)
    {
        //player->teleportTo(pos + Vector3(0.f, -speed, 0.f), false, 0, 0, false);
        player->GetAABBShapeComponent()->AAY -= speed;
        player->GetAABBShapeComponent()->BBY -= speed;
        player->GetStateVectorComponent()->velocity = Vector3(0.f, 0.f, 0.f);
        moveInputComponent->isSneakDown = false;
    }*/
    Block* block = Minecraft::ClientInstance->GetBlockSource()->getBlock(blockPos);
    bool isSolid = block->blockLegacy->isSolid;
    if (aabbCheck && isJumping && isSolid && !Misc::IsAirBlock(blockPos))
    {
        player->GetAABBShapeComponent()->AAY += speed;
        player->GetAABBShapeComponent()->BBY += speed;
        player->GetStateVectorComponent()->velocity = Vector3(0.f, 0.f, 0.f);
        moveInputComponent->isJumping = false;
    }

    if (player->isOnGround() && isSneaking || aabbCheck && isSneaking)
    {
        player->GetAABBShapeComponent()->AAY -= speed;
        player->GetAABBShapeComponent()->BBY -= speed;
        player->GetStateVectorComponent()->velocity = Vector3(0.f, 0.f, 0.f);
        moveInputComponent->isSneakDown = false;
    }



}

void Phase::OnDisable() {
    if (Mode.SelectedIndex != 0) return;
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    auto shapeComponent = player->GetAABBShapeComponent();
    if (!shapeComponent) return;

    shapeComponent->BBY = shapeComponent->AAY + oldHeight;

    if (WasBlinking)
    {
        RakPeerHooks::Ticking = false;
        RakPeerHooks::TicksToSkip = 0;
        RakPeerHooks::MsToSkip = 0;
        WasBlinking = false;
    }
}

void Phase::OnRender()
{
}