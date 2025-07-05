//
// Created by vastrakai on 1/27/2024.
//

#include "Surround.h"

#include <Prax/Module/Modules/Combat/Antibot.h>
#include <Prax/Utils/Math.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/Utils/Render/Render.h>

class PlayerAuthInputPacket;

bool Surround::Rotate = false;
Vector3 Surround::Block = Vector3();

Actor* Surround::GetClosestActor()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto actors = Antibot::GetActors();

    if (!player || actors.empty()) {
        return nullptr;
    }

    // get the closest actor
    Actor* closestActor = nullptr;
    float closestDistance = -1.f;
    for (auto actor : actors) {
        if (actor == player) continue;

        float distance = actor->getPos()->Distance(*player->getPos());
        if (closestDistance == -1.f || distance < closestDistance) {
            closestDistance = distance;
            closestActor = actor;
        }
    }



    if (!closestActor) {
        return nullptr;
    }

    // Distance check
    if (closestDistance > Distance.Value) {
        return nullptr;
    }

    return closestActor;

}

std::vector<BlockPos> Surround::GetCollidingBlocks()
{
    auto closestActor = GetClosestActor();
    if (!closestActor) return {};

    AABB aabb = closestActor->GetAABB();

    // Get all blocks that the actor is colliding with
    std::vector<BlockPos> collidingBlocks = std::vector<BlockPos>();
    Vector3 max = aabb.upper;
    Vector3 min = aabb.lower;
    max.x += AabbPadding.Value;
    max.z += AabbPadding.Value;
    min.x -= AabbPadding.Value;
    min.z -= AabbPadding.Value;
    if (PadY.Enabled)
    {
        /*max.y += AabbPadding.Value;
        min.y -= AabbPadding.Value;*/
    }

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

std::vector<BlockPos> Surround::GetSurroundingBlocks(bool airOnly)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto collidingBlocks = GetCollidingBlocks();

    std::vector<BlockPos> surroundingBlocks = std::vector<BlockPos>();

    for (auto blockPos : collidingBlocks) {
        surroundingBlocks.emplace_back(blockPos.x + 1, blockPos.y, blockPos.z);
        surroundingBlocks.emplace_back(BlockPos(blockPos.x - 1, blockPos.y, blockPos.z));
        surroundingBlocks.emplace_back(BlockPos(blockPos.x, blockPos.y, blockPos.z + 1));
        surroundingBlocks.emplace_back(BlockPos(blockPos.x, blockPos.y, blockPos.z - 1));
        surroundingBlocks.emplace_back(BlockPos(blockPos.x, blockPos.y + 1, blockPos.z));
        surroundingBlocks.emplace_back(BlockPos(blockPos.x, blockPos.y - 1, blockPos.z));
    }

    if (airOnly && DoubleThickness.Enabled)
    {
        std::vector<BlockPos> newBlocks = std::vector<BlockPos>();
        for (auto blockPos : surroundingBlocks)
        {
            if (Misc::IsAirBlock(blockPos)) continue;

            // Go through each side of the block
            for (int i = 0; i < 6; i++) {
                auto player = Minecraft::ClientInstance->ThePlayer();
                BlockPos blockSel = blockPos + Misc::GetSide(i);
                if (Misc::IsValidPlacePos(blockSel) && Misc::IsAirBlock(blockSel)) newBlocks.emplace_back(blockSel);
            }
        }

        // add newBLocks to surrounding
        for (auto blockPos : newBlocks)
        {
            surroundingBlocks.emplace_back(blockPos);
        }

    }

    // remove duplicates/blocks in the colliding blocks list
    for (auto collidingBlock : collidingBlocks) {
        for (int i = 0; i < surroundingBlocks.size(); i++) {
            if (surroundingBlocks[i] == collidingBlock) {
                surroundingBlocks.erase(surroundingBlocks.begin() + i);
                i--;
            }
        }
    }


    if (airOnly)
    {
        for (int i = 0; i < surroundingBlocks.size(); i++) {
            if (!Misc::IsAirBlock(surroundingBlocks[i])) {
                surroundingBlocks.erase(surroundingBlocks.begin() + i);
                i--;
            }
        }
    }

    // Sort
    switch (SortingMode.SelectedIndex) {
        case DISTANCE:
            std::sort(surroundingBlocks.begin(), surroundingBlocks.end(), [player](BlockPos a, BlockPos b) {
                return a.Distance(*player->getPos()) < b.Distance(*player->getPos());
            });
            break;
        case Y:
            std::sort(surroundingBlocks.begin(), surroundingBlocks.end(), [player](BlockPos a, BlockPos b) {
                return a.y < b.y;
            });
            break;
        case NONE:
        default:
            break;
    }


    return surroundingBlocks;
}

void Surround::OnTick()
{
    int places = PlacesPerTick;
    auto player = Minecraft::ClientInstance->ThePlayer();

    // for each place in tick, call Tick(0
    for (int i = 0; i < places; i++) {
        Tick();
    }
}

Vector3 getClosestPlacePos(Vector3 block, float maxDistance, Vector3 playerPos)
{
    if (Misc::IsValidPlacePos(block)) return block;

    for (int i = 2; i < 6; i++) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        BlockPos blockSel = block + Misc::GetSide(i);
        if (Misc::IsValidPlacePos(blockSel) && Misc::IsAirBlock(blockSel)) return blockSel;
    }



    playerPos = Vector3(0.f, 2.62f, 0.f);
    playerPos.x = floorf(playerPos.x);
    playerPos.y = floorf(playerPos.y);
    playerPos.z = floorf(playerPos.z);

    Vector3 closestBlock = Vector3(INT_MAX, INT_MAX, INT_MAX);
    float closestDistance = FLT_MAX;

    for (int x = block.x - maxDistance; x < block.x + maxDistance; x++) {
        for (int y = block.y - maxDistance; y < block.y + maxDistance; y++) {
            for (int z = block.z - maxDistance; z < block.z + maxDistance; z++) {
                Vector3 blockSel = Vector3(x, y, z);
                if (Misc::IsValidPlacePos(blockSel) && Misc::IsAirBlock(blockSel)) {
                    float distance = blockSel.Distance(playerPos);
                    if (distance < closestDistance) {
                        closestDistance = distance;
                        closestBlock = blockSel;
                    }
                }
            }
        }
    }

    if (closestBlock.x != INT_MAX && closestBlock.y != INT_MAX && closestBlock.z != INT_MAX) {
        return BlockPos(closestBlock.x, closestBlock.y, closestBlock.z);
    }

    for (int i = 0; i < 1; i++) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        BlockPos blockSel = block + Misc::GetSide(i);
        if (Misc::IsValidPlacePos(blockSel)) return blockSel;
    }

    return BlockPos(INT_MAX, INT_MAX, INT_MAX);
}

Vector3 Surround::GetNextPlacePos()
{
    static int placeIndex = 0;
    Vector3 closestBlock = Vector3();
    float closestDistance = -1.f;
    auto player = Minecraft::ClientInstance->ThePlayer();

    // Get the next block to place on
    Actor* closestActor = GetClosestActor();
    if (!closestActor) return {FLT_MAX, FLT_MAX, FLT_MAX};
    std::vector<BlockPos> surroundingBlocks = GetSurroundingBlocks(true);

    if (surroundingBlocks.empty()) return {FLT_MAX, FLT_MAX, FLT_MAX};

    // Validate place index
    if (placeIndex >= surroundingBlocks.size()) {
        placeIndex = 0;
    }

    // Set the closest block to the block at the current place index
    closestBlock = surroundingBlocks[placeIndex];
    placeIndex++;

    int side = Misc::GetBlockPlaceFace(closestBlock);
    if (side == -1)
    {
            /*
            // find the closest block to place on
            blockSel = Misc::GetClosestPlacePos(blockSel, DistanceSetting.Value, false, yPos);
            if (blockSel.x == INT_MAX) return defaultRet;
            side = Misc::GetBlockPlaceFace(blockSel);

            if (side == -1) return defaultRet;
            */

        closestBlock = getClosestPlacePos(closestBlock, Distance.Value, *closestActor->getPos());
        if (closestBlock.x == INT_MAX) return {FLT_MAX, FLT_MAX, FLT_MAX};
        side = Misc::GetBlockPlaceFace(closestBlock);

        if (side == -1) return {FLT_MAX, FLT_MAX, FLT_MAX};


    }

    return closestBlock;

}

void Surround::Tick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    Vector3 placePos = GetNextPlacePos();
    if (placePos.x == FLT_MAX) return;

    // Get the first stack of blocks
    int slot = Misc::GetPlaceableItemOnBlock(placePos, false, false);
    if (slot == -1) return;
    player->SelectSlot(slot);

    if (!Misc::IsValidPlacePos(placePos)) return;
    if (Misc::GetBlockPlaceFace(placePos) == -1) return;

    Rotate = true;
    Block = placePos;

    Misc::PlaceBlock(placePos, 0, false);
}

void Surround::OnEnable()
{

}

void Surround::OnDisable()
{

}








void Surround::OnRender()
{
    for (auto blockPos : GetCollidingBlocks())
    {
        Render::DrawBoxAroundBlockOutline(blockPos, ImColor(255, 0, 0, 255));
    }

    for (auto blockPos : GetSurroundingBlocks(false))
    {
        Render::DrawBoxAroundBlockOutline(blockPos, ImColor(0, 100, 255, 255));
    }
}

void Surround::OnPacket(LoopbackPacketSender* sender, Packet* packet, bool* cancel)
{
    if (packet->getId() == (int)PacketID::PlayerAuthInput)
    {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        if (Rotate && RotateSetting.Enabled)
        {
            auto paip = reinterpret_cast<PlayerAuthInputPacket*>(packet);
            Vector3 blockpos = Block;
            AABB aabb = AABB(blockpos, Vector3(1.f, 1.f, 1.f));
            Vector2 rots = Misc::GetRotations(*player->getPos(), aabb);
            paip->rotation = rots;

            Rotate = false;
        }


    }
}