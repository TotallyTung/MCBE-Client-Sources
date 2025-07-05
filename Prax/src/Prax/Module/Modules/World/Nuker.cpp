//
// Created by vastrakai on 2/24/2024.
//

#include "Nuker.h"

#include <Prax/Utils/Render/Render.h>
#include <Prax/Hook/Hooks/RakPeerHooks.h>
#include <Prax/SDK/Game/Network/Other/InventoryTransactionPacket.h>
#include <Prax/SDK/Game/Network/Other/PlayerActionPacket.h>

BlockPosPair Nuker::CurrentBlock = BlockPosPair(Vector3(), nullptr);
float Nuker::Progress = 0.f;
bool Nuker::Rotate = false;
bool Nuker::Breaking = false;

void Nuker::Reset()
{
    CurrentBlock = BlockPosPair(Vector3(), nullptr);
    Progress = 0.f;
    Rotate = false;
    Breaking = false;
}


Vector3 getStandingPos()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    Vector3 pos = *player->getPos() - Vector3(0.f, 2.62, 0.f);

    float dist = 1.f;

    std::vector<Vector3> posvec;

    for (int x = -dist; x < dist; x++)
    {
        for (int z = -dist; z < dist; z++)
        {
            Vector3 newPos = Vector3(pos.x + x, pos.y, pos.z + z);
            // Floor the position
            newPos = Vector3(floor(newPos.x), floor(newPos.y), floor(newPos.z));
            if (!Misc::IsAirBlock(newPos))
                posvec.emplace_back(newPos);
        }
    }

    // sort by distance to player
    std::sort(posvec.begin(), posvec.end(), [&pos](Vector3 a, Vector3 b) {
        return a.Distance(pos) < b.Distance(pos);
    });

    if (posvec.size() == 0) return pos;

    pos = posvec.at(0);

    return pos;
}

#ifndef __RELEASEFREE__

// A block that can be instantly broken by FatNgga mode
struct BreakableBlock {
    int toolId;
    int blocKId;
};

struct BreakableBlockM {
    int blocKId;
    std::vector<int> toolIds;
};

// Tools that can break blocks
constexpr int DIAMOND_PICKAXE = 278;
constexpr int DIAMOND_SHOVEL = 277;
constexpr int DIAMOND_AXE = 279;
constexpr int DIAMOND_SWORD = 276;
constexpr int IRON_PICKAXE = 257;
constexpr int IRON_SHOVEL = 256;
constexpr int IRON_AXE = 258;
constexpr int IRON_SWORD = 267;
constexpr int GOLD_PICKAXE = 285;
constexpr int GOLD_SHOVEL = 284;
constexpr int GOLD_AXE = 286;
constexpr int GOLD_SWORD = 283;
constexpr int STONE_PICKAXE = 274;
constexpr int STONE_SHOVEL = 273;
constexpr int STONE_AXE = 275;
constexpr int STONE_SWORD = 272;
constexpr int WOODEN_PICKAXE = 270;
constexpr int WOODEN_SHOVEL = 269;
constexpr int WOODEN_AXE = 271;
constexpr int WOODEN_SWORD = 268;

static std::vector<BreakableBlock> BreakableBlocks = {
        {300, 155}, // iron_pickaxe -> tile.quartz_block
        {300, 24}, // iron_pickaxe -> tile.sandstone
        {300, 99}, // iron_pickaxe -> tile.brown_mushroom_block

        {326, 573}, // golden_shovel -> tile.dirt_with_roots
        {326, 976}, // golden_shovel -> tile.concretePowder.green
        {326, 970}, // golden_shovel -> tile.concretePowder.gray
        {326, 82}, // golden_shovel -> tile.clay
        {326, 2}, // golden_shovel -> tile.grass
        {326, 3}, // golden_shovel -> tile.dirt
        {326, 12}, // golden_shovel -> tile.sand
        {326, 13}, // golden_shovel -> tile.gravel
        {326, 243}, // golden_shovel -> tile.podzol


        {319, 863}, // diamond_sword -> tile.carpet.brown
        {319, 928}, // diamond_sword -> tile.stained_glass.orange
        {319, 1057}, // diamond_sword -> tile.leaves.jungle
        {319, 1057}, // diamond_sword -> tile.leaves.jungle
        {319, 18}, // diamond_sword -> tile.leaves.oak
        {319, 38}, // diamond_sword -> tile.red_flower
        {319, 31}, // diamond_sword -> tile.tallgrass
        {319, 39}, // diamond_sword -> tile.brown_mushroom
        {319, 574}, // diamond_sword -> tile.hanging_roots
        {319, 32}, // diamond_sword -> tile.deadbush
        {319, 841}, // diamond_sword -> tile.coral.pink_dead
        {319, 1055}, // diamond_sword -> tile.leaves.spruce
        {319, 31}, // diamond_sword -> tile.tallgrass
        {319, 171}, // diamond_sword -> tile.carpet.white
        {319, 860}, // diamond_sword -> tile.carpet.cyan

        {321, 847}, // diamond_pickaxe -> tile.stone.diorite
        {321, 849}, // diamond_pickaxe -> tile.stone.andesite
        {321, 845}, // diamond_pickaxe -> tile.stone.granite
        {321, 1}, // diamond_pickaxe -> tile.stone.stone
        {321, 387}, // diamond_pickaxe -> tile.coral_block
        {321, 490}, // diamond_pickaxe -> tile.polished_basalt
        {321, 572}, // diamond_pickaxe -> tile.dripstone_block
        {321, 24}, // diamond_pickaxe -> tile.sandstone
        {321, 213}, // diamond_pickaxe -> tile.magma
        {321, 155}, // diamond_pickaxe -> tile.quartz_block
        {321, 987}, // diamond_pickaxe -> tile.stained_hardened_clay.cyan
        {321, 983}, // diamond_pickaxe -> tile.stained_hardened_clay.lime
        {321, 991}, // diamond_pickaxe -> tile.stained_hardened_clay.green
        {321, 985}, // diamond_pickaxe -> tile.stained_hardened_clay.gray
        {321, 172}, // diamond_pickaxe -> tile.hardened_clay
        {321, 846}, // diamond_pickaxe -> tile.stone.graniteSmooth
        {321, 528}, // diamond_pickaxe -> tile.blackstone
        {321, 536}, // diamond_pickaxe -> tile.gilded_blackstone
        {321, 850}, // diamond_pickaxe -> tile.stone.andesiteSmooth
        {321, 429}, // diamond_pickaxe -> tile.polished_andesite_stairs

};

std::map<Vector3, uint64_t> blockBreakTimes;

bool Nuker::HasToolForBlock(int blockId, std::vector<ItemStack*> items)
{
    // Go through the inventory and check if we have a tool for the block
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;

    for (auto item : items)
    {
        if (!item->item) continue;
        Item* item2 = *item->item;
        if (!item2) continue;

        for (auto block : BreakableBlocks)
        {
            if (block.blocKId == blockId && block.toolId == item2->id)
            {
                return true;
            }
        }
    }

    return false;
}

bool Nuker::IsBlockWithinYaw(BlockPos bPos, float playerYaw, float yawRange) {
    // the yaw in this case is in degrees, from -180 to 180

    // Floor and center the blockpos
    Vector3 blockpos = Vector3((float)floor(bPos.x) + 0.5f, (float)floor(bPos.y) + 0.5f, (float)floor(bPos.z) + 0.5f);

    float yaw = Misc::GetRotations(Minecraft::ClientInstance->ThePlayer()->getPos(), &blockpos).y;

    // Normalize the yaw
    if (yaw < 0) yaw += 360.f;

    // Normalize the playerYaw
    if (playerYaw < 0) playerYaw += 360.f;

    // Calculate the difference between the playerYaw and the blockYaw
    float diff = abs(playerYaw - yaw);

    // If the difference is more than 180, subtract 360
    if (diff > 180) diff = 360 - diff;

    // If the difference is less than the yawRange, return true
    return diff < yawRange;
}

bool Nuker::IsBlockWithinPitch(BlockPos bPos, float playerPitch, float pitchRange) {
    // the pitch in this case is in degrees, from -90 to 90

    // Floor and center the blockpos
    Vector3 blockpos = Vector3((float)floor(bPos.x) + 0.5f, (float)floor(bPos.y) + 0.5f, (float)floor(bPos.z) + 0.5f);

    float pitch = Misc::GetRotations(Minecraft::ClientInstance->ThePlayer()->getPos(), &blockpos).x;

    // Normalize the pitch
    if (pitch < 0) pitch += 360.f;

    // Normalize the playerPitch
    if (playerPitch < 0) playerPitch += 360.f;

    // Calculate the difference between the playerPitch and the blockPitch
    float diff = abs(playerPitch - pitch);

    // If the difference is more than 180, subtract 360
    if (diff > 180) diff = 360 - diff;

    // If the difference is less than the pitchRange, return true
    return diff < pitchRange;
}

void Nuker::StartDestroy(Vector3 pos, int face) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    bool outBool = false;
    /*auto startPkt = PlayerActionPacket::Create(PlayerAction::StartDestroyBlock, pos, pos, face, player->GetRuntimeID(), false);
    Minecraft::ClientInstance->packetSender->sendToServer(startPkt.get());*/
    player->gamemode->startDestroyBlock(pos, face, outBool);
}

void Nuker::DestroyBlock(Vector3 pos, int face, int slot) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    auto pkt = MinecraftPackets::CreatePacket<InventoryTransactionPacket>();

    auto cit = std::make_unique<ItemUseInventoryTransaction>();
    cit->actionType = ItemUseInventoryTransaction::ActionType::Destroy;
    cit->slot = slot;
    cit->itemInHand = *player->supplies->inventory->getItem(slot);
    cit->blockPos = pos;
    cit->face = face;
    cit->targetBlockRuntimeId = 0; // Infinite durability?!?!?!
    cit->playerPos = *player->getPos();
    cit->clickPos = pos;

    pkt->mTransaction = std::move(cit);

    Minecraft::ClientInstance->packetSender->sendToServer(pkt.get());
}

bool Nuker::IsExposed(Vector3 pos){
    if (!Underground.Enabled) {
        return Misc::GetExposedBlockFace(pos) != -1;
    }

    std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(pos);

    bool isExposed = false;

    bool redstoneFound = false;

    for (auto& block : surroundingBlocks) {
        // Return true if the block is air
        if (block.second.block->blockLegacy->id == 0) return true;
        if (block.second.block->blockLegacy->id == 73 || block.second.block->blockLegacy->id == 74) redstoneFound = true;
    }

    // if the block at the pos is redstone here then return false
    auto block = Minecraft::ClientInstance->GetBlockSource()->getBlock(BlockPos(pos));
    if (block->blockLegacy->id == 73 || block->blockLegacy->id == 74) return false;
    if (redstoneFound) return false;

    return true;
}

void Nuker::ModeFatNggaTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;


    static bool rightClickold = false;
    bool rightClick = GetAsyncKeyState(VK_RBUTTON);

    if (!Debug.Enabled) goto skipDebug;

    if (rightClick && !rightClickold)
    {
        // Get the block in the level hitresult
        HitResult* hitResult = player->level->getHitResult();
        if (!hitResult) return;

        // goto skipDebug if the hitResult is not a block
        if (hitResult->type != RaycastTypes::BLOCK) goto skipDebug;

        Block* block = Minecraft::ClientInstance->GetBlockSource()->getBlock(hitResult->blockPos);
        if (!block) return;

        // Get the block id
        int blockId = block->blockLegacy->id;

        // Get the currently held item id
        ItemStack* heldStack = player->supplies->inventory->getItem(player->supplies->selectedHotbarSlot);
        if (!heldStack) return;

        if (!heldStack->item) return;
        Item* heldItem = *heldStack->item;

        int heldItemId = heldItem->id;

        int bestTool = Misc::GetBestBreakingTool(block);
        ItemStack* stack = player->supplies->inventory->getItem(bestTool);
        if (!stack->item) return;

        Item* item = *stack->item;

        // calculate the blocks breaking ms with the best tool
        float destroySpeed = Misc::GetDestroySpeed(bestTool, block, DestroySpeed.Value);
        uint64_t blockBreakTime = Misc::GetBlockBreakTime(destroySpeed);
        uint64_t breakingMs = blockBreakTime - Misc::GetCurrentMs();

        player->addChatMessage("Block: " + std::to_string(blockId) + " (" + block->blockLegacy->tileName + ") Tool: " + std::to_string(heldItemId) + " (" + heldItem->GetName() + ") Best Tool: " + std::to_string(item->id) + " (" + item->GetName() + ") Breaking Time: " + std::to_string(breakingMs) + "ms", false);
        Logger::Write(__FUNCTION__, "Block: " + std::to_string(blockId) + " (" + block->blockLegacy->tileName + ") Tool: " + std::to_string(heldItemId) + " (" + heldItem->GetName() + ") Best Tool: " + std::to_string(item->id) + " (" + item->GetName() + ") Breaking Time: " + std::to_string(breakingMs) + "ms", Logger::Error);

        Misc::SetClipboard("{" + std::to_string(heldItemId) + ", " + std::to_string(blockId) + "}, // " + heldItem->GetName() + " -> " + block->blockLegacy->tileName);
    }
    rightClickold = rightClick;
    return;

    skipDebug:

    // Remove blocks from the blockBreakTimes map that are older than 1 second, or are air
    std::erase_if(blockBreakTimes, [](std::pair<Vector3, uint64_t> pair) {
        return pair.second < Misc::GetCurrentMs() - 1000 || Misc::IsAirBlock(pair.first);
    });

    bool isNetSkipping = RakPeerHooks::Ticking;

    if (isNetSkipping) {
        player->addChatMessage("§cCannot use FatNgga mode while net skipping§r", false);
        return;
    }

    Vector3 pos = *player->getPos();
    float range = Range.Value;

    std::vector<BlockPosPair> blocks = Misc::GetBlocksWithinDistance2(player->getPos(), Range.Value, false);

    std::vector<ItemStack*> items = player->supplies->GetInventoryItems();

    std::vector<Vector3> blocksToBreak;
    float playerYaw = player->GetActorRotationComponent()->yaw;
    for (auto block : blocks)
    {
        // Check if the center of the block is within the distance
        if (block.GetCenteredPos().Distance(pos) > range) continue;

        // Check if the block is in the breakable blocks list
        bool found = false;
        for (auto b : BreakableBlocks)
        {
            if (b.blocKId == block.block->blockLegacy->id)
            {
                found = true;
                break;
            }
        }
        if (!found) continue;

        if (block.blockPos == getStandingPos()) continue;

        // Calculate rotation. If the rotation yaw is more than 38, continue
        Vector3 blockpos = block.blockPos;
        // Floor the blockpos and center it
        blockpos = Vector3(floor(blockpos.x) + 0.5f, floor(blockpos.y) + 0.5f, floor(blockpos.z) + 0.5f);

        if (!RotateSetting.Enabled) {
            if (!IsBlockWithinYaw(blockpos, playerYaw, 30.f)) continue;
            if (!IsBlockWithinPitch(blockpos, player->GetActorRotationComponent()->pitch, 30.f)) continue;
        }

        bool exposed = IsExposed(block.blockPos);
        if (!exposed) continue;

        if (!HasToolForBlock(block.block->blockLegacy->id, items)) continue;

        blocksToBreak.emplace_back(block.blockPos);
        if (blocksToBreak.size() >= (int)BlocksToBreak.Value) break;
    }

    // Go through the list of BreakableBlocks and make sure we have the tools necessary. If we dont, remove all block ids with that tool



    if (blocksToBreak.size() == 0)
    {
        Logger::Write(__FUNCTION__, "No blocks to break", Logger::Error);
        return;
    }

    for (auto block : blocksToBreak)
    {
        int face = Misc::GetExposedBlockFace(block);

        Logger::Write(__FUNCTION__, "Breaking block", Logger::Error);
        StartDestroy(block, face);
        player->gamemode->breakDuration = 1.f;
        BlockPosPair blockPair = BlockPosPair(block, Minecraft::ClientInstance->GetBlockSource()->getBlock(block));
        int toolSlot = -1;
        // Get the toolSlot assocciated with this block by looking thru inventory
        for (int i = 0; i < items.size(); i++)
        {
            ItemStack* stack = items.at(i);
            if (!stack->item) continue;
            Item* item = *stack->item;
            if (!item) continue;

            for (auto b : BreakableBlocks)
            {
                if (b.blocKId == blockPair.block->blockLegacy->id && b.toolId == item->id)
                {
                    toolSlot = i;
                    break;
                }
            }
        }

       /* float destroySpeed = Misc::GetDestroySpeed(toolSlot, blockPair.block, 1.f);
        uint64_t blockBreakTime = Misc::GetBlockBreakTime(destroySpeed);
        uint64_t breakingMs = blockBreakTime - Misc::GetCurrentMs();

        // Display a warning if the breakingMs is more than 250ms
        if (breakingMs > 250)
        {
            player->addChatMessage("§cWarning: Block with name &b" + blockPair.block->blockLegacy->tileName + "&7[" + std::to_string(blockPair.block->blockLegacy->id) + "]&c will take &e" + std::to_string(breakingMs) + "ms&c to break", true);
            Toggle(); // Disable the module to prevent us from getting kicked
        }*/

       // Continue if we already have tried to break this block in the past 200ms
        if (blockBreakTimes.find(block) != blockBreakTimes.end() && blockBreakTimes[block] > Misc::GetCurrentMs() - 200) continue;

        DestroyBlock(blockPair.blockPos, face, toolSlot);

        blockBreakTimes[block] = Misc::GetCurrentMs();

        if (RotateSetting.Enabled) {
            Rotate = true;
            CurrentBlock = blockPair;
        }

    }
}
#endif


void Nuker::ModeLegitTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    Vector3 pos = *player->getPos();
    float range = Range.Value;


    if (Breaking && Misc::IsAirBlock(CurrentBlock.blockPos)) {
        Logger::Write(__FUNCTION__, "Block is air", Logger::Error);
        Reset();
        return;
    }

    if (Breaking && CurrentBlock.block != nullptr)
    {
        // check if the block is still exposed, and if the closest point on the block is within distance
        if (CurrentBlock.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos()) > Range.Value && Breaking)
        {
            Logger::Write(__FUNCTION__, "Block is out of range", Logger::Error);
            Reset();
            return;
        }

        if (Progress < 1.f)
        {
            float destroySpeed = Misc::GetDestroySpeed(Misc::GetBestBreakingTool(CurrentBlock.block), CurrentBlock.block, DestroySpeed.Value);
            // Minimum speed should be 0.08
            if (destroySpeed < 0.08f) destroySpeed = 0.08f;
            Progress += destroySpeed;
            Logger::Write(__FUNCTION__, "Breaking block " + std::to_string(Progress), Logger::Error);
            return;
        }

        if (Progress >= 1.f) {
            // Finish mining the block

            player->gamemode->breakDuration = 1.f;

            Rotate = true;

            int oldSlot = player->supplies->selectedHotbarSlot;
            player->SelectSlot(Misc::GetBestBreakingTool(CurrentBlock.block));

            bool oldSwing = player->isSwinging;
            int oldSwimgTime = player->swingProgress;
            player->gamemode->destroyBlock(CurrentBlock.blockPos, 0);
            if (!oldSwing && !Swing.Enabled) player->isSwinging = false;
            if (oldSwing) player->swingProgress = oldSwimgTime;

            player->SelectSlot(oldSlot);

            player->gamemode->breakDuration = 0.f;

            Breaking = false;
            Progress = 0.f;

            Logger::Write(__FUNCTION__, "Finished breaking block", Logger::Error);
            return;
        }

        return;
    }


    std::vector<BlockPosPair> blocks = Misc::GetBlocksWithinDistance2(player->getPos(), Range.Value);

    // Sort by distance
    if (Sorting.SelectedIndex == (int)SortingMode::DISTANCE)
            std::sort(blocks.begin(), blocks.end(), [&pos](BlockPosPair a, BlockPosPair b) {
                return a.blockPos.Distance(pos) < b.blockPos.Distance(pos);
            });


    // Remove blocks that are not exposed
        std::erase_if(blocks, [](BlockPosPair block) {
        return Misc::GetExposedBlockFace(block.blockPos) == -1;
    });

    // Remove the block the player is standing on
    std::erase_if(blocks, [&pos](BlockPosPair block) {
        return block.blockPos == getStandingPos();
    });



    if (blocks.size() == 0)
    {
        Logger::Write(__FUNCTION__, "No blocks to break", Logger::Error);
        return;
    }

    BlockPosPair targetBlock = blocks.at(0);




    bool outBool = false;
    player->gamemode->startDestroyBlock(targetBlock.blockPos, Misc::GetExposedBlockFace(targetBlock.blockPos), outBool);
    if (outBool)
    {
        Rotate = true;
        CurrentBlock = targetBlock;
        Logger::Write(__FUNCTION__, "Instabreaking block", Logger::Error);
        return;
    }
    player->gamemode->continueDestroyBlock(targetBlock.blockPos, Misc::GetExposedBlockFace(targetBlock.blockPos), *player->getPos(), outBool);
    if (outBool)
    {
        Rotate = true;
        CurrentBlock = targetBlock;
        Logger::Write(__FUNCTION__, "Instabreaking block", Logger::Error);
        return;
    }

    Breaking = true;
    CurrentBlock = targetBlock;

    Logger::Write(__FUNCTION__, "Starting to break block", Logger::Error);




}

void Nuker::OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    auto id = (PacketID)packet->getId();

    if (id == PacketID::PlayerAuthInput) {
        auto paip = (PlayerAuthInputPacket*)packet;

        if (!paip->playerBlockActions.mActions.empty())
        {
            for (PlayerBlockActionData action : paip->playerBlockActions.mActions)
            {

            }
        }

        if (Rotate)
        {
            auto blockAabb = CurrentBlock.GetAABB();
            Vector2 rotations = Misc::GetRotations(*player->getPos(), blockAabb);
            paip->rotation = rotations;
            paip->headYaw = rotations.y;
            Rotate = false;
        }
    }

}

void Nuker::OnRender()
{
#ifndef __RELEASEFREE__
    if (!Render.Enabled) return;
    // If !breaking and mode is not FatNgga, return
    if (!Breaking && Mode.SelectedIndex != (int)NukerMode::FATNGGA) return;

    // Draw a box around each block in blockBreakTimes
    for (auto pair : blockBreakTimes)
    {
        Vector3 pos = pair.first;
        uint64_t time = pair.second;

        ImColor color = ImColor(1.f, 1.f, 1.f, 1.f);

        // make the color flash red and yellow
        if (time < Misc::GetCurrentMs() + 500)
        {
            color = ImColor(1.f, 0.f, 0.f, 0.4f);
        }
        else if (time < Misc::GetCurrentMs() + 1000)
        {
            color = ImColor(1.f, 1.f, 0.f, 0.4f);
        }

        // fade the alpha to 0 from 0.4 based on time
        color.Value.w = 0.4f - (float)(Misc::GetCurrentMs() - time) / 1000.f;



        Render::DrawBoxAroundBlock(pos, color, color);
    }

    Vector3 pos = CurrentBlock.blockPos;
    Render::DrawBoxAroundBlockOutline(pos, ImColor(1.f, 1.f, 1.f, 1.f));
#endif
}

void Nuker::ModeInstantTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    Vector3 pos = *player->getPos();
    float range = Range.Value;

    for (int x = -range; x < range; x++)
    {
        for (int y = -range; y < range; y++)
        {
            for (int z = -range; z < range; z++)
            {
                Vector3 blockPos = Vector3(pos.x + x, pos.y + y, pos.z + z);

                player->gamemode->breakDuration = 1.f;
                player->gamemode->destroyBlock(blockPos, 1);
                player->gamemode->breakDuration = 0.f;
            }
        }
    }
}

void Nuker::OnEnable()
{
    Reset();
}

void Nuker::OnDisable()
{

}

void Nuker::OnTick()
{
    if (Mode.SelectedIndex == (int)NukerMode::INSTANT)
    {
        ModeInstantTick();
    }
    else if (Mode.SelectedIndex == (int)NukerMode::LEGIT)
    {
        ModeLegitTick();
    }
#ifndef __RELEASEFREE__
    else if (Mode.SelectedIndex == (int)NukerMode::FATNGGA)
    {
        ModeFatNggaTick();
    }
#endif
}
