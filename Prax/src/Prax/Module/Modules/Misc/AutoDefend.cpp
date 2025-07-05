//
// Created by vastrakai on 1/28/2024.
//

#include "AutoDefend.h"

#include <Prax/Module/Modules/Player/Teams.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/Utils/Render/Render.h>

bool AutoDefend::Rotate = false;
Vector3 AutoDefend::Block = Vector3();

bool IsAboveDistance(Vector3* pos) {
    return pos->Distance(*Minecraft::ClientInstance->ThePlayer()->getPos()) > 8.0f;
}

Actor* AutoDefend::GetTreasure()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    std::vector<Actor*> actors = player->level->getRuntimeActorList();

    std::erase_if(actors, [](auto actor) {
        return actor->GetEntityId() != "hivewars:treasure";
    });

    // Remove actors that are too far away (more than 8 blocks)
    actors.erase(std::ranges::remove_if(actors, [](Actor* actor) {
        return IsAboveDistance(actor->getPos());
    }).begin(), actors.end());

    // remove actors not on the same team using Teams::IsOnTeam
    actors.erase(std::ranges::remove_if(actors, [](Actor* actor) {
        return !Teams::IsOnTeam(actor);
    }).begin(), actors.end());

    if (actors.empty()) {
        return nullptr;
    }

    return actors.front();
}


std::vector<BlockPos> AutoDefend::GetCollidingBlocks()
{
    auto closestActor = GetTreasure();
    if (!closestActor) return {};

    AABB aabb = closestActor->GetAABB();

    // Get all blocks that the actor is colliding with
    std::vector<BlockPos> collidingBlocks = std::vector<BlockPos>();
    Vector3 max = aabb.upper;
    Vector3 min = aabb.lower;

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

std::vector<BlockPos> AutoDefend::GetSurroundingBlocks(bool airOnly)
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

    if (airOnly)
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


    // Remove duplicates
    std::ranges::sort(surroundingBlocks, [player](BlockPos a, BlockPos b) {
        return a.y < b.y;
    });

    return surroundingBlocks;
}


Vector3 AutoDefend::getClosestPlacePos(Vector3 block, float maxDistance, Vector3 playerPos)
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


Vector3 AutoDefend::GetNextPlacePos()
{
    static int placeIndex = 0;
    Vector3 closestBlock = Vector3();
    float closestDistance = -1.f;
    auto player = Minecraft::ClientInstance->ThePlayer();

    // Get the next block to place on
    Actor* closestActor = GetTreasure();
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

        closestBlock = getClosestPlacePos(closestBlock, 7.f, *closestActor->getPos());
        if (closestBlock.x == INT_MAX) return {FLT_MAX, FLT_MAX, FLT_MAX};
        side = Misc::GetBlockPlaceFace(closestBlock);

        if (side == -1) return {FLT_MAX, FLT_MAX, FLT_MAX};
    }

    return closestBlock;

}

void AutoDefend::OnEnable()
{

}

void AutoDefend::OnTick()
{
    int places = PlacesPerTick;
    auto player = Minecraft::ClientInstance->ThePlayer();

    // for each place in tick, call Tick(0
    for (int i = 0; i < places; i++) {
        Tick();
    }
}

void AutoDefend::Tick()
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

void AutoDefend::OnPacket(LoopbackPacketSender* sender, Packet* packet, bool* cancel)
{
    if (packet->getId() == (int)PacketID::PlayerAuthInput)
    {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        if (Rotate)
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

void AutoDefend::OnRender()
{
    if (!Minecraft::ClientInstance->levelRenderer) return;
    for (auto blockPos : GetCollidingBlocks())
    {
        Render::DrawBoxAroundBlockOutline(blockPos, ImColor(255, 0, 0, 255));
    }

    for (auto blockPos : GetSurroundingBlocks(false))
    {
        Render::DrawBoxAroundBlockOutline(blockPos, ImColor(0, 100, 255, 255));
    }
}
