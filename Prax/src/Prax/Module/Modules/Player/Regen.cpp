//
// Created by vastrakai on 12/31/2023.
//

#include "Regen.h"

#include <Prax/Module/ModuleManager.h>
#include <Prax/SDK/Game/Network/Other/LevelEventPacket.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/D2D.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/SDK/Game/Network/Other/PlayerActionPacket.h>
#include <Prax/SDK/Game/Network/Other/InventoryTransactionPacket.h>
#include <Prax/Hook/Hooks/RakPeerHooks.h>
#include <Prax/Module/Modules/Visual/HUD.h>
#include <Prax/SDK/Game/Network/Other/UpdateBlockPacket.h>
#include <Prax/Module/Modules/World/Scaffold.h>

bool Regen::IsBreaking = false;
float Regen::Progress = 0.0f;
BlockPosPair Regen::CurrentBlock = BlockPosPair(Vector3(), nullptr);
bool Regen::ShouldRotate = false;
bool Regen::StartStealing = false;
BlockPosPair Regen::StealBlock = BlockPosPair(Vector3(), nullptr);
bool Regen::IsStealing = false;
uint64_t Regen::StealStartTime = 0;
int Regen::StealSide = -1;
std::vector<std::pair<uint64_t, Vector3>> Regen::RecentlyCoveredBlocks = {};

void Regen::Reset()
{
    if (IsBreaking) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        // Cancel mining
        //player->gamemode->stopDestroyBlock(CurrentBlock.blockPos);
    }

    if (IsBreaking)
    {
        REGEN_AddDebugMessage("Reset was called");
    }
    IsBreaking = false;
    IsStealing = false;
    StealStartTime = 0;
    Progress = 0.0f;


}

bool Regen::ShouldQueueOres()
{
#ifndef __RELEASEFREE__
    return QueueOres.Enabled;
#else
    return false;
#endif
}

std::vector<int> oreIds = {
        56, 129, 14, 15, 21, 16, 416, 658, 659, 660, 662, 657, 656, 655, 661
};

float Regen::GetBbDestroySpeed(Block* block)
{
    if (!block) return DestroySpeed.Value;
    bool isRedstone = block->blockLegacy->id == 73 || block->blockLegacy->id == 74;
    if (isRedstone) return DestroySpeed.Value;
    bool isOre = std::ranges::find(oreIds, block->blockLegacy->id) != oreIds.end();
    if (isOre) return OreDestroySpeed.Value;
    return OtherDestroySpeed.Value;
}

// simple wrapping method for gamemode start destroy block
void Regen::StartBreaking(Vector3 blockPos, int blockFace, bool& outBool)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    outBool = false;

    /*player->gamemode->startDestroyBlock(blockPos, blockFace, outBool);
    if (outBool) return;
    player->gamemode->continueDestroyBlock(blockPos, blockFace, *player->getPos(), outBool);
    if (outBool) return;*/

    if (StartBreakMethod.SelectedIndex == (int)StartBreakMethod::AuthInputActions)
    {
        player->gamemode->startDestroyBlock(blockPos, blockFace, outBool);
        if (outBool) return;
        player->gamemode->continueDestroyBlock(blockPos, blockFace, *player->getPos(), outBool);
        return;
    }


    {
        auto packet = MinecraftPackets::CreatePacket<PlayerActionPacket>();
        packet->mRuntimeId = player->getRuntimeIDComponent()->runtimeID;
        packet->mAction = PlayerAction::StartDestroyBlock;
        packet->mPos = blockPos;
        AABB blockAABB = BlockPosPair(blockPos, Minecraft::ClientInstance->GetBlockSource()->getBlock(blockPos)).GetAABB();
        packet->mResultPos = blockAABB.GetClosestPoint(*player->getPos());
        packet->mFace = blockFace;
        Minecraft::ClientInstance->packetSender->sendToServer(packet.get());
    }

    {
        // continue
        auto packet = MinecraftPackets::CreatePacket<PlayerActionPacket>();
        packet->mRuntimeId = player->getRuntimeIDComponent()->runtimeID;
        packet->mAction = PlayerAction::ContinueDestroyBlock;
        packet->mPos = blockPos;
        AABB blockAABB = BlockPosPair(blockPos, Minecraft::ClientInstance->GetBlockSource()->getBlock(blockPos)).GetAABB();
        packet->mResultPos = blockAABB.GetClosestPoint(*player->getPos());
        packet->mFace = blockFace;
        Minecraft::ClientInstance->packetSender->sendToServer(packet.get());
    }


}

std::map<uint64_t, Regen::QueuedPacket> Regen::QueuedPackets = {};

void Regen::DestroyBlock(Vector3 blockPos, int blockFace)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
#ifndef __RELEASEFREE__
    if (!InfiniteDurability.Enabled) {
        player->gamemode->destroyBlock(blockPos, blockFace);
        player->gamemode->stopDestroyBlock(blockPos);
    }
    else {
        auto pkt = MinecraftPackets::CreatePacket<InventoryTransactionPacket>();

        auto cit = std::make_unique<ItemUseInventoryTransaction>();
        cit->actionType = ItemUseInventoryTransaction::ActionType::Destroy;
        int slot = player->supplies->selectedHotbarSlot;
        cit->slot = slot;
        cit->itemInHand = *player->supplies->inventory->getItem(slot);
        cit->blockPos = blockPos;
        cit->face = blockFace;
        cit->targetBlockRuntimeId = InfiniteFunnies.Enabled ? 3690217760 : 0;
        cit->playerPos = *player->getPos();

        BlockPosPair block = BlockPosPair(blockPos, Minecraft::ClientInstance->GetBlockSource()->getBlock(blockPos));
        AABB blockAABB = block.GetAABB();

        cit->clickPos = blockAABB.GetClosestPoint(*player->getPos());
        pkt->mTransaction = std::move(cit);

        QueuedPackets[Misc::GetCurrentMs()] = {blockPos, pkt, slot};
        if (Swing.Enabled) player->swing();
    }
#else
    player->gamemode->destroyBlock(blockPos, blockFace);
#endif


}

bool Regen::IsExposed(Vector3 pos)
{
    std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(pos);

    bool isExposed = false;

    bool redstoneFound = false;

    for (auto& block : surroundingBlocks) {
        // Return true if the block is air
        if (block.second.block->blockLegacy->id == 0) return true;

#ifndef __RELEASEFREE__
        if (ExcludePlacedBlocks.Enabled) {
            // Check if the passed in pos was covered in the last 3 seconds, and return true if so (bypass)
            for (auto& coveredBlock : RecentlyCoveredBlocks) {
                if (coveredBlock.second == pos) return true;
            }
        }
#endif

        // Return true if the block is water
        if (String::ContainsIgnoreCase(block.second.block->blockLegacy->name, "water")) return true;
        if (String::ContainsIgnoreCase(block.second.block->blockLegacy->name, "lava")) return true;
        // Return true if the block is a chest
        if (block.second.block->blockLegacy->id == 54) return true;
        //if (!block.second.block->blockLegacy->isSolid) return true;
        if (block.second.block->blockLegacy->id == 73 || block.second.block->blockLegacy->id == 74) redstoneFound = true;
    }

    // if the block at the pos is redstone here then return false
    auto block = Minecraft::ClientInstance->GetBlockSource()->getBlock(BlockPos(pos));
    if (block->blockLegacy->id == 73 || block->blockLegacy->id == 74) return false;
    if (redstoneFound) return false;

    return true;
}

static int64_t dabreakingMs = 0;
static int64_t dablockBreakTime = 0;

void Regen::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

#ifndef __RELEASEFREE__
    bool noUncoverWhileStealing = NoUncoverWhileStealing.Enabled;
    if (NoUncoverRandomized.Enabled) {
        static bool lastUncover = false;
        lastUncover = !lastUncover;

        noUncoverWhileStealing = lastUncover;
    }
#endif

    // Remove recently placed blocks from more than 3000ms ago
    for (auto it = RecentlyCoveredBlocks.begin(); it != RecentlyCoveredBlocks.end();) {
        if (Misc::GetCurrentMs() - it->first > 3000) {
            it = RecentlyCoveredBlocks.erase(it);
        } else {
            ++it;
        }
    }

    for (auto& packet : QueuedPackets) {
        if (packet.first <= Misc::GetCurrentMs()) {
            auto action = MinecraftPackets::CreatePacket<PlayerActionPacket>();
            action->mRuntimeId = player->getRuntimeIDComponent()->runtimeID;
            action->mAction = PlayerAction::StopDestroyBlock;
            action->mPos = packet.second.pos;
            action->mFace = 0;
            Misc::SpoofSwitch(packet.second.slot);
            Minecraft::ClientInstance->packetSender->sendToServer(action.get());
            Minecraft::ClientInstance->packetSender->sendToServer(packet.second.packet.get());
            Minecraft::ClientInstance->GetBlockSource()->clearBlock(packet.second.pos);

            QueuedPackets.erase(packet.first);
            REGEN_AddDebugMessage("Sent queued packet");
        }
    }

    if (player->getStatusFlag(ActorFlags::Noai)) return;

    auto stolenProgress = 0.f;

    // Check if the current block is air and Reset if it is
    if (IsBreaking && Misc::IsAirBlock(CurrentBlock.blockPos)) {
        REGEN_AddDebugMessage("Current block is air, resetting");
        if(AliensDetection.Enabled) player->addChatMessage("&6[Regen] &cCurrent block was stolen!", true);
        stolenProgress = Progress;
        Reset();
        return;
    }

    static auto scaffoldModule = ModuleManager::GetModule<Scaffold>();
    if (scaffoldModule->Enabled)
    {
        Reset();
        return;
    }


    float absorption = player->getAbsorption();
#ifndef __RELEASEFREE__
    if (!AbsorptionCheck.Enabled) absorption = 0.f;
#endif
    bool hasMaxAbsorption = absorption >= 10.f;
    bool shouldQueueOres = ShouldQueueOres();
    if (hasMaxAbsorption && !shouldQueueOres)
    {
        Reset();
        return;
    }

    float maxDistance = Distance.Value;
#ifndef __RELEASEFREE__
    if (StartStealing && StealOres.Enabled)
    {
        REGEN_AddDebugMessage("Stealing ore");
        StartStealing = false;
        IsStealing = true;
        StealStartTime = Misc::GetCurrentMs();

        BlockPosPair targetBlock = StealBlock;

        bool outBool = false;
        StartBreaking(targetBlock.blockPos, targetBlock.block->blockLegacy->id, outBool);
        if (outBool)
        {
            REGEN_AddDebugMessage("&cInstantly mined " + targetBlock.block->blockLegacy->name + " [outBool == true]");
            ShouldRotate = true;
            CurrentBlock = targetBlock;
            return;
        }

        float destroySpeed = Misc::GetDestroySpeed(Misc::GetBestBreakingTool(targetBlock.block), targetBlock.block, GetBbDestroySpeed(targetBlock.block));


        // Check if the destroySpeed is 1.0 and continue if it is (the block was mined instantly)
        if (destroySpeed == 1.0f)
        {
            REGEN_AddDebugMessage("&cInstantly mined " + targetBlock.block->blockLegacy->name + " [destroySpeed == 1.0f]");
            ShouldRotate = true;
            CurrentBlock = targetBlock;
            return;
        }

#ifndef __RELEASEFREE__

        // Check if the block is 0.27 or higher and set the destroySpeed to 1.0
        float realdestroySpeed = Misc::GetDestroySpeed(Misc::GetBestBreakingTool(targetBlock.block), targetBlock.block, 1.f);
        uint64_t nonAdjustedBlockBreakTime = Misc::GetBlockBreakTime(realdestroySpeed);
        uint64_t nonAdjustedBreakingMs = nonAdjustedBlockBreakTime - Misc::GetCurrentMs();

        if (nonAdjustedBreakingMs <= (uint64_t)FunnyMaxMs.Value && InstabreakFunny.Enabled)
        {
            // destroy the block NOW
            bool oldSwing = player->isSwinging;
            int oldSwimgTime = player->swingProgress;
            int oldSlot = player->supplies->selectedHotbarSlot;
            player->SelectSlot(Misc::GetBestBreakingTool(targetBlock.block));
            DestroyBlock(targetBlock.blockPos, 0);
            if (!oldSwing && !Swing.Enabled) player->isSwinging = false;
            if (oldSwing) player->swingProgress = oldSwimgTime;
            player->SelectSlot(oldSlot);
            ShouldRotate = true;
            CurrentBlock = targetBlock;
            REGEN_AddDebugMessage("&cInstantly mined " + targetBlock.block->blockLegacy->name + " [destroySpeedMs <= " + std::to_string((uint64_t)FunnyMaxMs.Value) + "] [InstabreakFunny.Enabled]");
            return;

        }

#endif

        IsBreaking = true;
        CurrentBlock = targetBlock;

        if (DestroySpeedMode.SelectedIndex == (int)DestroySpeedMode::Milliseconds && (CurrentBlock.block->blockLegacy->id == 73 || CurrentBlock.block->blockLegacy->id == 74))
        {
            int64_t speedMs = DestroySpeedMilliseconds.Value;
            if (speedMs <= 0) speedMs = 1;
            dablockBreakTime = (int64_t)(speedMs) + Misc::GetCurrentMs();
            dabreakingMs = (int64_t)speedMs;

            int64_t timeLeft = dablockBreakTime - Misc::GetCurrentMs();
        }
        else {
            dablockBreakTime = Misc::GetBlockBreakTime(destroySpeed);
            dabreakingMs = dablockBreakTime - Misc::GetCurrentMs();
        }

        REGEN_AddDebugMessage("&cMining " + CurrentBlock.block->blockLegacy->name + " in " + std::to_string(dabreakingMs) + "ms (non-adjusted: " + std::to_string(nonAdjustedBreakingMs) + "ms)");
        return;
    }

    if (!IsExposed(CurrentBlock.blockPos) && !IsStealing && ResetWhenCovered.Enabled)
    {
        if(AliensDetection.Enabled) player->addChatMessage("&6[Regen] &eWARNING: &cCurrent block was covered!", true);
        CurrentBlock = BlockPosPair(Vector3(), nullptr);
        Reset();
        return;
    }
    else if (!IsExposed(CurrentBlock.blockPos) && !IsStealing && ResetWhenCoveredQueued.Enabled && hasMaxAbsorption)
    {
        if(AliensDetection.Enabled) player->addChatMessage("&6[Regen] &eWARNING: &cCurrent block was covered! &7[3]&f", true);
        CurrentBlock = BlockPosPair(Vector3(), nullptr);
        Reset();
        return;
    }
    else if (!ResetWhenCovered.Enabled && !IsStealing) {
        static bool lastExposed = true;

        bool isExposed = IsExposed(CurrentBlock.blockPos);
        if (!isExposed && lastExposed) {
            if(AliensDetection.Enabled) player->addChatMessage("&6[Regen] &eWARNING: &cCurrent block was covered! &7[2]&f", true);
        }

        lastExposed = isExposed;
    }

    // check if we have a current block and log if we are
#else
    if (!IsExposed(CurrentBlock.blockPos))
    {
        CurrentBlock = BlockPosPair(Vector3(), nullptr);
        Reset();
        return;
    }
#endif

    if (IsBreaking && CurrentBlock.block != nullptr) {
        bool isRegenOre = CurrentBlock.block->blockLegacy->id == 73 || CurrentBlock.block->blockLegacy->id == 74;



#ifndef __RELEASEFREE__
        // check if the block is still exposed, and if the closest point on the block is within distance
        if (CurrentBlock.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos()) > maxDistance && isRegenOre)
        {
            CurrentBlock = BlockPosPair(Vector3(), nullptr);
            Reset();
            return;
        }
        std::vector<BlockPosPair> blocks = Misc::GetBlocksWithinDistance2(player->getPos(), Distance.Value);
        BlockPosPair targetBlock = BlockPosPair(Vector3(), nullptr);
        bool found = FindFunnyBlock(blocks, targetBlock, Distance.Value, maxDistance);

        if (found) {
            if (!IsExposed(CurrentBlock.blockPos) && !IsStealing && !isRegenOre && CurrentBlock.blockPos != targetBlock.blockPos) {
                CurrentBlock = BlockPosPair(Vector3(), nullptr);
                Reset();
                return;
            }
        } else {

            if (!IsExposed(CurrentBlock.blockPos) && !IsStealing && !isRegenOre) {
                CurrentBlock = BlockPosPair(Vector3(), nullptr);
                Reset();
                return;
            }
        }
#else
        // check if the block is still exposed, and if the closest point on the block is within distance
        if (CurrentBlock.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos()) > Distance.Value && isRegenOre)
        {
            CurrentBlock = BlockPosPair(Vector3(), nullptr);
            Reset();
            return;
        }

        if (!IsExposed(CurrentBlock.blockPos))
        {
            CurrentBlock = BlockPosPair(Vector3(), nullptr);
            Reset();
            return;
        }
#endif




        // Check if the player is still mining the block
        if (Progress < 1.f)
        {
            float destroySpeed = Misc::GetDestroySpeed(Misc::GetBestBreakingTool(CurrentBlock.block), CurrentBlock.block, GetBbDestroySpeed(CurrentBlock.block));
            // If the block is water, set destroy speed to 0.20f
            if (String::ContainsIgnoreCase(CurrentBlock.block->blockLegacy->name, "water")) destroySpeed = 0.20f;
            if (String::ContainsIgnoreCase(CurrentBlock.block->blockLegacy->name, "lava")) destroySpeed = 0.20f;

            // if isRegenOre and DestroySpeedMode is set to milliseconds, calculate the progress based on the blockBreakTime
            // else calculate the progress based on the destroySpeed


            if (isRegenOre && DestroySpeedMode.SelectedIndex == (int)DestroySpeedMode::Milliseconds) {
                // Calculate the progress based on the blockBreakTime
                int64_t timeLeft = dablockBreakTime - Misc::GetCurrentMs();

                if (RakPeerHooks::Ticking) {
                    if (RakPeerHooks::TicksToSkip != 0) {
                        if ((RakPeerHooks::TicksToSkip - RakPeerHooks::CurrentTick) * 50 < timeLeft && RakPeerHooks::TicksToSkip * 50 > timeLeft) {
                            RakPeerHooks::TicksToSkip = 0;
                            RakPeerHooks::MsToSkip = timeLeft;
                            player->addChatMessage("the eeper,,,");
                        } else {
                            Logger::Write("Regen|NetSkip", "Ticking: " + std::to_string(RakPeerHooks::Ticking) + " TicksToSkip: " + std::to_string(RakPeerHooks::TicksToSkip) + " CurrentTick: " + std::to_string(RakPeerHooks::CurrentTick) + " MsToSkip: " + std::to_string(RakPeerHooks::MsToSkip) + " timeLeft: " + std::to_string(timeLeft), Logger::Warning);
                        }
                    }
                    // TODO: ms netskip
                }

                if (dablockBreakTime < Misc::GetCurrentMs()) {
                    timeLeft = 0;
                }

                Progress = 1.f - (float)timeLeft / (float)dabreakingMs;
                Progress = std::clamp(Progress, 0.f, 1.f);
            } else if (Minecraft::ClientInstance->GetTimer() == 20 && Minecraft::ClientInstance->GetTimerMultiplier() == 1.f)
                Progress += destroySpeed;
            else
            {
                uint64_t timeLeft = dablockBreakTime - Misc::GetCurrentMs();

                if (dablockBreakTime < Misc::GetCurrentMs()) {
                    timeLeft = 0;
                }

                Progress = 1.f - (float)timeLeft / (float)dabreakingMs;
                Progress = std::clamp(Progress, 0.f, 1.f);
            }
        }



#ifndef __RELEASEFREE__
        if (Progress < 1.f && InfiniteDurability.Enabled && BreakSpeedAdjustment.Enabled)
        {
            float destroySpeed = Misc::GetDestroySpeed(Misc::GetBestBreakingTool(CurrentBlock.block), CurrentBlock.block, GetBbDestroySpeed(CurrentBlock.block));
            // If we are going to break the block next tick set progress to 1
            if (Progress + destroySpeed >= 1.f) Progress = 1.f;
        }

        if (AlwaysSteal.Enabled && IsStealing) hasMaxAbsorption = false;

        bool finishBreaking = Progress >= 1.f && !hasMaxAbsorption;
        // if the block isnt redstone, ignore the absorption check
        if (!isRegenOre) finishBreaking = Progress >= 1.f;

        // If the StealStartTime is more than 1.5 seconds ago and the block is still not broken, reset
        if (IsStealing && Misc::GetCurrentMs() - StealStartTime > 700 && isRegenOre) {
            REGEN_AddDebugMessage("Stealer timed out");
            Reset(); // do NOT break the redstone here, you WILL get 7 dayed
            return;
        }

        if (IsStealing && !IsExposed(CurrentBlock.blockPos) && isRegenOre) finishBreaking = false;

        float distance = CurrentBlock.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos());
#else
        bool finishBreaking = Progress >= 1.f;
#endif

        if (finishBreaking) {
            // Finish mining the block

            player->gamemode->breakDuration = 1.f;

            ShouldRotate = true;

            int oldSlot = player->supplies->selectedHotbarSlot;
            player->SelectSlot(Misc::GetBestBreakingTool(CurrentBlock.block));
            bool oldSwing = player->isSwinging;
            int oldSwimgTime = player->swingProgress;
            //player->gamemode->destroyBlock(CurrentBlock.blockPos, 0);
            DestroyBlock(CurrentBlock.blockPos, 0);
            if (!oldSwing && !Swing.Enabled) player->isSwinging = false;
            if (oldSwing) player->swingProgress = oldSwimgTime;
            player->SelectSlot(oldSlot);

            player->gamemode->breakDuration = 0.f;

            REGEN_AddDebugMessage("Finished mining " + CurrentBlock.block->blockLegacy->name);

            Reset();
        }

        return;
    }

    std::vector<BlockPosPair> blocks = Misc::GetBlocksWithinDistance2(player->getPos(), maxDistance);

    BlockPosPair targetBlock = BlockPosPair(Vector3(), nullptr);

    // Get the closest exposed redstone ore
    for (auto& block : blocks) {
        // Check if the block id is 73 or 74
        bool isRedstone = block.block->blockLegacy->id == 73 || block.block->blockLegacy->id == 74;

        if (isRedstone && IsExposed(block.blockPos)) {

            // Continue if the block isnt in range of the player
            if (block.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos()) > Distance.Value) continue;
            targetBlock = block;
            break;
        }
    }

    bool funnyBlockening = false;

    if (targetBlock.block == nullptr)
    {
#ifndef __RELEASEFREE__
        bool mineUnder = UndergroundBypass.Enabled;
        if (StealOres.Enabled && noUncoverWhileStealing) mineUnder = false;

        if (mineUnder)
        {

            bool foundExposed = false;

            for (BlockPosPair block : blocks) {
                // Check if the block id is 73 or 74
                if (bool isRedstone = block.block->blockLegacy->id == 73 || block.block->blockLegacy->id == 74; !isRedstone) continue;

                // Continue if the block isnt in range of the player
                if (block.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos()) > Distance.Value) continue;

                // Check if there are any blocks surrounding this redstone ore that are exposed
                std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(block.blockPos, false);


                for (auto& block : surroundingBlocks) {
                    if (IsExposed(block.second.blockPos)) {
                        targetBlock = block.second;
                        foundExposed = true;
                        break;
                    }
                }

                if (foundExposed) break;
            }

            if (!foundExposed && mineUnder)
            {
                if (UncoverSorting.SelectedIndex == (int)UncoverSorting::Distance) {
                    for (BlockPosPair block: blocks) {
                        // Check if the block id is 73 or 74
                        if (bool isRedstone = block.block->blockLegacy->id == 73 ||
                                              block.block->blockLegacy->id == 74; !isRedstone)
                            continue;

                        // Continue if the block isnt in range of the player
                        if (block.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos()) >
                            Distance.Value)
                            continue;

                        // Check if there are any blocks surrounding this redstone ore that are exposed
                        std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(block.blockPos, 1);

                        for (auto &block: surroundingBlocks) {
                            if (IsExposed(block.second.blockPos)) {
                                targetBlock = block.second;
                                foundExposed = true;
                                break;
                            }
                        }
                    }
                } else if (UncoverSorting.SelectedIndex == (int)UncoverSorting::DestroySpeed) {
                    std::vector<std::pair<BlockPosPair, float>> blockSpeeds = {};

                    for (BlockPosPair block: blocks) {
                        // Check if the block id is 73 or 74
                        if (bool isRedstone = block.block->blockLegacy->id == 73 ||
                                              block.block->blockLegacy->id == 74; !isRedstone)
                            continue;

                        // Continue if the block isnt in range of the player
                        if (block.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos()) >
                            Distance.Value)
                            continue;

                        // Check if there are any blocks surrounding this redstone ore that are exposed
                        std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(block.blockPos, 1);

                        for (auto &block: surroundingBlocks) {
                            if (IsExposed(block.second.blockPos)) {
                                // Add the block to the blockSpeeds vector
                                blockSpeeds.push_back({block.second, Misc::GetDestroySpeed(Misc::GetBestBreakingTool(block.second.block), block.second.block, 1.f)});
                            }
                        }

                        if (foundExposed) break;
                    }

                    if (blockSpeeds.size() > 0) {
                        std::sort(blockSpeeds.begin(), blockSpeeds.end(), [](const std::pair<BlockPosPair, float>& a, const std::pair<BlockPosPair, float>& b) {
                            return a.second < b.second;
                        });

                        targetBlock = blockSpeeds[0].first;
                        foundExposed = true;
                    }
                }



                if (!foundExposed && TheFunny.Enabled && mineUnder) {
                    foundExposed = FindFunnyBlock(blocks, targetBlock, Distance.Value, maxDistance);

                    if (foundExposed) {
                        REGEN_AddDebugMessage("&eFound funny block: " + targetBlock.block->blockLegacy->name);
                        funnyBlockening = true; // It does exactly what you think it does.
                    }
                }

                if (!foundExposed && UncoverSorting.SelectedIndex == (int)UncoverSorting::Distance && mineUnder) {
                    for (BlockPosPair block: blocks) {
                        // Check if the block id is 73 or 74
                        if (bool isRedstone = block.block->blockLegacy->id == 73 ||
                                              block.block->blockLegacy->id == 74; !isRedstone)
                            continue;

                        // Continue if the block isnt in range of the player
                        if (block.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos()) >
                            Distance.Value)
                            continue;

                        // Check if there are any blocks surrounding this redstone ore that are exposed
                        std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(block.blockPos,
                                                                                                   UncoverMode.SelectedIndex);

                        for (auto &block: surroundingBlocks) {
                            if (IsExposed(block.second.blockPos)) {
                                targetBlock = block.second;
                                foundExposed = true;
                                break;
                            }
                        }
                    }
                } else if (!foundExposed && UncoverSorting.SelectedIndex == (int)UncoverSorting::DestroySpeed && mineUnder) {
                    std::vector<std::pair<BlockPosPair, float>> blockSpeeds = {};
                    for (BlockPosPair block: blocks) {
                        // Check if the block id is 73 or 74
                        if (bool isRedstone = block.block->blockLegacy->id == 73 ||
                                              block.block->blockLegacy->id == 74; !isRedstone)
                            continue;

                        // Continue if the block isnt in range of the player
                        if (block.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos()) >
                            Distance.Value)
                            continue;

                        // Check if there are any blocks surrounding this redstone ore that are exposed
                        std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(block.blockPos,
                                                                                                   UncoverMode.SelectedIndex);

                        for (auto &block: surroundingBlocks) {
                            if (IsExposed(block.second.blockPos)) {
                                // Add the block to the blockSpeeds vector
                                blockSpeeds.push_back({block.second, Misc::GetDestroySpeed(Misc::GetBestBreakingTool(block.second.block), block.second.block, 1.f)});
                            }
                        }
                    }

                    if (blockSpeeds.size() > 0) {
                        std::sort(blockSpeeds.begin(), blockSpeeds.end(), [](const std::pair<BlockPosPair, float>& a, const std::pair<BlockPosPair, float>& b) {
                            return a.second < b.second;
                        });

                        targetBlock = blockSpeeds[0].first;
                        foundExposed = true;
                    }
                }
            }
        }

        if (targetBlock.block == nullptr)
        {
            Reset();
            return;
        }
#else
        // Free uncover is only 1 block uncover
        if (FreeUncover.Enabled)
        {
            bool foundExposed = false;

            if (UncoverSorting.SelectedIndex == (int)UncoverSorting::Distance)
            {
                for (BlockPosPair block : blocks)
                {
                    // Check if the block id is 73 or 74
                    if (bool isRedstone = block.block->blockLegacy->id == 73 || block.block->blockLegacy->id == 74; !isRedstone) continue;

                    // Continue if the block isnt in range of the player
                    if (block.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos()) > Distance.Value) continue;

                    // Check if there are any blocks surrounding this redstone ore that are exposed
                    std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(block.blockPos, UncoverMode.SelectedIndex + 1);

                    for (auto& bl : surroundingBlocks)
                    {
                        if (IsExposed(bl.second.blockPos))
                        {
                            targetBlock = bl.second;
                            foundExposed = true;
                            break;
                        }
                    }

                    if (foundExposed) break;
                }
            }
            else if (UncoverSorting.SelectedIndex == (int)UncoverSorting::DestroySpeed)
            {
                std::vector<std::pair<BlockPosPair, float>> blockSpeeds = {};

                for (BlockPosPair block : blocks)
                {
                    // Check if the block id is 73 or 74
                    if (bool isRedstone = block.block->blockLegacy->id == 73 || block.block->blockLegacy->id == 74; !isRedstone) continue;

                    // Continue if the block isnt in range of the player
                    if (block.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos()) > Distance.Value) continue;

                    // Check if there are any blocks surrounding this redstone ore that are exposed
                    std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(block.blockPos, 0);

                    for (auto& bl : surroundingBlocks)
                    {
                        if (IsExposed(bl.second.blockPos))
                        {
                            // Add the block to the blockSpeeds vector
                            blockSpeeds.emplace_back( bl.second, Misc::GetDestroySpeed(Misc::GetBestBreakingTool(bl.second.block), bl.second.block, 1.f) );
                        }
                    }

                    if (foundExposed) break;
                }

                if (!blockSpeeds.empty())
                {
                    std::ranges::sort(blockSpeeds, [](const std::pair<BlockPosPair, float>& a, const std::pair<BlockPosPair, float>& b) {
                        return a.second < b.second;
                    });

                    targetBlock = blockSpeeds[0].first;
                    foundExposed = true;
                }
            }

            if (!foundExposed)
            {
                Reset();
                return;

            }
        }
#endif
    }


    //if (targetBlock.block != nullptr) {
    //    Progress = stolenProgress;
    //}

    // Start mining the block if it is exposed and its not a funny block

    if (!IsExposed(targetBlock.blockPos) && !funnyBlockening)
    {
        Reset();
        return;
    }
    bool outBool = false;
    StartBreaking(targetBlock.blockPos, targetBlock.block->blockLegacy->id, outBool);
    if (outBool)
    {
        REGEN_AddDebugMessage("&cInstantly mined " + targetBlock.block->blockLegacy->name + " [outBool == true]");
        ShouldRotate = true;
        CurrentBlock = targetBlock;
        return;
    }

    float destroySpeed = Misc::GetDestroySpeed(Misc::GetBestBreakingTool(targetBlock.block), targetBlock.block, GetBbDestroySpeed(targetBlock.block));


    // Check if the destroySpeed is 1.0 and continue if it is (the block was mined instantly)
    if (destroySpeed == 1.0f)
    {
        REGEN_AddDebugMessage("&cInstantly mined " + targetBlock.block->blockLegacy->name + " [destroySpeed == 1.0f]");
        ShouldRotate = true;
        CurrentBlock = targetBlock;
        return;
    }


    // Check if the block is 0.27 or higher and set the destroySpeed to 1.0
    float realdestroySpeed = Misc::GetDestroySpeed(Misc::GetBestBreakingTool(targetBlock.block), targetBlock.block, 1.f);
    uint64_t nonAdjustedBlockBreakTime = Misc::GetBlockBreakTime(realdestroySpeed);
    uint64_t nonAdjustedBreakingMs = nonAdjustedBlockBreakTime - Misc::GetCurrentMs();


#ifndef __RELEASEFREE__

    if (nonAdjustedBreakingMs <= (uint64_t)FunnyMaxMs.Value && InstabreakFunny.Enabled)
    {
        // destroy the block NOW
        bool oldSwing = player->isSwinging;
        int oldSwimgTime = player->swingProgress;
        int oldSlot = player->supplies->selectedHotbarSlot;
        player->SelectSlot(Misc::GetBestBreakingTool(targetBlock.block));
        DestroyBlock(targetBlock.blockPos, 0);
        if (!oldSwing && !Swing.Enabled) player->isSwinging = false;
        if (oldSwing) player->swingProgress = oldSwimgTime;
        player->SelectSlot(oldSlot);
        ShouldRotate = true;
        CurrentBlock = targetBlock;
        REGEN_AddDebugMessage("&cInstantly mined " + targetBlock.block->blockLegacy->name + " [destroySpeedMs <= " + std::to_string((uint64_t)FunnyMaxMs.Value) + "]" + " [InstabreakFunny.Enabled]");
        return;

    }

#endif

    IsBreaking = true;
    CurrentBlock = targetBlock;


    if (DestroySpeedMode.SelectedIndex == (int)DestroySpeedMode::Milliseconds && (CurrentBlock.block->blockLegacy->id == 73 || CurrentBlock.block->blockLegacy->id == 74))
    {
        int64_t speedMs = DestroySpeedMilliseconds.Value;
        if (speedMs <= 0) speedMs = 1;
        dablockBreakTime = (int64_t)(speedMs) + Misc::GetCurrentMs();
        dabreakingMs = (int64_t)speedMs;

        int64_t timeLeft = dablockBreakTime - Misc::GetCurrentMs();
    }
    else {
        dablockBreakTime = Misc::GetBlockBreakTime(destroySpeed);
        dabreakingMs = dablockBreakTime - Misc::GetCurrentMs();
    }

    REGEN_AddDebugMessage("&cMining " + CurrentBlock.block->blockLegacy->name + " in " + std::to_string(dabreakingMs) + "ms (non-adjusted: " + std::to_string(nonAdjustedBreakingMs) + "ms)");
}

void Regen::OnEnable()
{

}

void Regen::OnDisable()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;



    // cancel mining
    if (IsBreaking) {
        // Cancel mining
        //player->gamemode->stopDestroyBlock(CurrentBlock.blockPos);
        IsBreaking = false;
        Progress = 0.0f;
        CurrentBlock = BlockPosPair(Vector3(), nullptr);
        ShouldRotate = false;
    }

    Reset();
}

void Regen::OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    auto id = (PacketID)packet->getId();

    if (id == PacketID::PlayerAuthInput) {
        auto paip = (PlayerAuthInputPacket*)packet;

        if (ShouldRotate)
        {
            auto blockAabb = CurrentBlock.GetAABB();
            Vector2 rotations = Misc::GetRotations(*player->getPos(), blockAabb);
            paip->rotation = rotations;
            paip->headYaw = rotations.y;
            ShouldRotate = false;
        }
    }
}

void Regen::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel)
{
#ifndef __RELEASEFREE__
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    PacketID id = (PacketID)packet->getId();

    if (id == PacketID::Disconnect)
    {
        // clear queued packets
        QueuedPackets.clear();
    }

    if (id == PacketID::UpdateBlock) {
        auto ubp = std::reinterpret_pointer_cast<UpdateBlockPacket>(packet);

        if (ubp->BlockRuntimeId != 3690217760) { // if not air runtime id
            Vector3 pos = ubp->Pos;
            std::map<int, BlockPosPair> blocks = Misc::GetSurroundingBlocks(pos, false);

            for (auto& block : blocks) {
                if (block.second.block->blockLegacy->id == 73 || block.second.block->blockLegacy->id == 74) {
                    int exposedSides = Misc::CountExposedSides(block.second.blockPos);
                    if (exposedSides != 0) continue;
                    RecentlyCoveredBlocks.emplace_back(Misc::GetCurrentMs(), block.second.blockPos);
                    Logger::Write("Regen|Covered", "Block covered: " + block.second.block->blockLegacy->name + " [exposedSides: " + std::to_string(exposedSides) + "]",
                                  Logger::Debug);
                    return;

                }
            }
        }
    }

    if (id == PacketID::LevelEvent) {
        auto lep = std::reinterpret_pointer_cast<LevelEventPacket>(packet);

        if (StealOres.Enabled)
        {
            // Set the current block if 3600 and pos is at ore
            if (lep->eventId == 3600)
            {
                auto pos = lep->pos;
                // Make sure this isnt the current block
                if (pos == CurrentBlock.blockPos) return;
                // Range check from lep->pos to lp->getPos()
                if (pos.Distance(*player->getPos()) > Distance.Value) return;

                std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(pos);
                Vector3 redstoneOre = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
                int oreSide = -1;

                for (auto& block : surroundingBlocks) {
                    if (block.second.block->blockLegacy->id == 73 || block.second.block->blockLegacy->id == 74) {
                        redstoneOre = block.second.blockPos;
                        oreSide = block.first;
                        break;
                    }
                }

                if (redstoneOre == Vector3(FLT_MAX, FLT_MAX, FLT_MAX)) return;
                if (redstoneOre == CurrentBlock.blockPos) return;

                bool outBool = false;
                REGEN_AddDebugMessage("Stealing ore [OnPacketReceive]");
                StartStealing = true;
                StealBlock = BlockPosPair(redstoneOre, Minecraft::ClientInstance->GetBlockSource()->getBlock(redstoneOre));
                StealSide = oreSide;
                Progress = 0.0f;


            }
        }
    }
#endif
}


static ImColor color = ImColor(0, 255, 0, 255);

void Regen::OnRenderBlock(){
    static float lastProgress = 0.f;
    if(RenderBlock.Enabled && IsBreaking) {
        ImColor c = color;
        c.Value.w = 0.5f;
        bool isRedstone = CurrentBlock.block->blockLegacy->id == 73 || CurrentBlock.block->blockLegacy->id == 74;
        // if it isnt redstone then make the color gray
        float progress = 1.f;
        Vector3 targetedBlock = CurrentBlock.blockPos;


        progress = Progress;
        // Lerp the progress with lastProgress and use Minecraft::ClientInstance->GetDeltaTime() to make it smooth
        if (progress < lastProgress) lastProgress = progress;
        progress = Math::Lerp(lastProgress, progress, ImGui::GetIO().DeltaTime * 30.f);
        lastProgress = progress;

        // Clamp the progress to 0-1
        progress = std::clamp(progress, 0.f, 1.f);

        if (progress < 0.02f)
        {

            return;
        }

        // make sure the pos is adjusted for progress so it's still centered (If the progress is 1, it will be centered)
        AABB fullAabb = AABB(targetedBlock, Vector3(1, 1, 1));
        targetedBlock.x += 0.5f - (progress / 2.f);
        targetedBlock.y += 0.5f - (progress / 2.f);
        targetedBlock.z += 0.5f - (progress / 2.f);

        AABB aabb = AABB(targetedBlock, Vector3(progress, progress, progress));

        Math::DrawBoxFilled(aabb.lower, aabb.upper, 1.0f, c, c, false);
        //Math::DrawOutlinedBox(fullAabb.lower, fullAabb.upper, 1.0f, c);
    }
}

float Regen::AnimationPerc = 0.f;

void Regen::OnRenderProgress()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    float lastAnimationPerc = AnimationPerc;
    static float lastProgress = 0.f;
    if (!RenderProgress.Enabled)
    {
        return;
    }
    // Get the middle of the screen
    ImVec2 pos = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2.5f);
    pos.y += pos.y / 2;
    pos.y += ProgressYOffset.Value;

    // Render the progress text
    bool isRedstone = false;

    if (CurrentBlock.block && CurrentBlock.block->blockLegacy) isRedstone = CurrentBlock.block->blockLegacy->id == 73 || CurrentBlock.block->blockLegacy->id == 74;

#ifndef __RELEASEFREE__
    bool isQueuedOre = ShouldQueueOres() && isRedstone && player->getAbsorption() >= 10.f && AbsorptionCheck.Enabled;
#else
    bool isQueuedOre = ShouldQueueOres() && isRedstone && player->getAbsorption() >= 10.f;
#endif
    bool isUncoveringRedstone = !isRedstone;
    float progress = 1.f;


    progress = Progress;
    // Lerp the progress with lastProgress and use Minecraft::ClientInstance->GetDeltaTime() to make it smooth
    progress = Math::Lerp(lastProgress, progress, ImGui::GetIO().DeltaTime * 30.f);
    lastProgress = progress;

    // Clamp the progress to 0-1
    progress = std::clamp(progress, 0.f, 1.f);


    static std::string text = "Mining redstone";
    float absoprtion = player->getAbsorption();

    static ImColor oldColor = color;
    float lerpSpeed = 12.f;

    float sped = ImGui::GetIO().DeltaTime * lerpSpeed;

#ifndef __RELEASEFREE__
    if (isQueuedOre && Enabled && IsStealing && isRedstone && IsBreaking) {
        color = ImColor(235, 110, 52, 255);
        text = xorstr_("Stealing redstone");

        AnimationPerc = Math::Lerp(AnimationPerc, 1.f, sped);
    } if (isQueuedOre && Enabled && IsBreaking)
    {
        color = ImColor(0, 255, 255, 255);
        if (progress < 1.f) text = xorstr_("Queueing ore");
        else text = xorstr_("Queued ore");

        AnimationPerc = Math::Lerp(AnimationPerc, 1.f, sped);
    } else
#endif
    if (isUncoveringRedstone && Enabled && IsBreaking)
    {
        color = ImColor(255, 0, 0, 255);
        text = xorstr_("Uncovering redstone");

        AnimationPerc = Math::Lerp(AnimationPerc, 1.f, sped);
    }
#ifndef __RELEASEFREE__
    else if (Enabled && isRedstone && IsBreaking && IsStealing)
    {
        color = ImColor(235, 110, 52, 255);
        text = xorstr_("Stealing redstone");

        AnimationPerc = Math::Lerp(AnimationPerc, 1.f, sped);
    }
#endif
    else if (IsBreaking && isRedstone && Enabled)
    {
        color = ImColor(0, 255, 0, 255);
        text = xorstr_("Mining redstone");

        AnimationPerc = Math::Lerp(AnimationPerc, 1.f, sped);
    } else
    {
        AnimationPerc = Math::Lerp(AnimationPerc, 0.f, sped);
    }

    if (AnimationPerc <= 0.01f || text.empty()) return;
    float colorLerpSpeed = 7.5f;

    float speed = 1.5f;
    float fromT = 0.5f;
    float toT = 1.0f;

    static float wave = 1.f;

    // Make the wave only apply when max progess is reached
    if (progress >= 1.f)
    {
        wave = Math::Lerp(fromT, toT, (sin(ImGui::GetTime() * speed) + 1.0f) / 2.0f);
        color.Value.x = color.Value.x * wave;
        color.Value.y = color.Value.y * wave;
        color.Value.z = color.Value.z * wave;
    } else
    {
        wave = 1.f;
    }

    // Lerp the color
    color = ImColor(
            Math::Lerp(oldColor.Value.x, color.Value.x, ImGui::GetIO().DeltaTime * colorLerpSpeed),
            Math::Lerp(oldColor.Value.y, color.Value.y, ImGui::GetIO().DeltaTime * colorLerpSpeed),
            Math::Lerp(oldColor.Value.z, color.Value.z, ImGui::GetIO().DeltaTime * colorLerpSpeed),
            Math::Lerp(oldColor.Value.w, color.Value.w, ImGui::GetIO().DeltaTime * colorLerpSpeed)
    );

    oldColor = color;

    // Make the color wave darker and lighter
    // Wave from 0.5 to 1.0 and from 1.0 to 0.5


#ifndef __RELEASEFREE__
    if (NaClVisuals.Enabled) {
        const auto y = ImGui::GetIO().DisplaySize.y / 2 + 30;
        Render::DrawGlowLine(ImVec2(pos.x - 25, y), ImVec2(pos.x - 25 + (50 * progress), y), color, 1.8);
    } else
#endif
    {
        ImGui::PushFont(Font::Fonts["greycliff_demibold_large"]);

        // Render the text
        float fontSize = ProgressSize.Value;
        // min fontSize should be 5 hardcoded
        if (fontSize < 3) fontSize = 3;
        fontSize *= AnimationPerc;

        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, text.c_str());
        pos.x -= textSize.x / 2;
        pos.y -= textSize.y / 2;

        float rounding = 5.f;
        float bgAlpha = 0.45f;

        if (progress > 0.f)
            ImGui::GetBackgroundDrawList()->AddShadowRect(ImVec2(pos.x - 5, pos.y - 5), ImVec2(pos.x + textSize.x * progress + 5, pos.y + textSize.y + 5), ImColor(color.Value.x, color.Value.y, color.Value.z, bgAlpha * AnimationPerc), 500, ImVec2(0, 0));


        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(pos.x - 5, pos.y - 5), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 5), ImColor(0.f, 0.f, 0.f, (bgAlpha * AnimationPerc)), rounding, 0);

        // Fill in the rest of the progress
        if (progress > 0.f)
        {

            //if (progress < 0.99f) ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(pos.x + (textSize.x * (progress + 0.02)) + 5, pos.y - 5), ImVec2(pos.x + textSize.x + 5, pos.y + textSize.y + 5), ImColor(0.f, 0.f, 0.f, (bgAlpha * AnimationPerc)), rounding, 0);

            // Colored progress
            ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(pos.x - 5, pos.y - 5), ImVec2(pos.x + textSize.x * progress + 5, pos.y + textSize.y + 5), ImColor(color.Value.x, color.Value.y, color.Value.z, bgAlpha * AnimationPerc), rounding, 0);

        }

        Render::RenderText(text, pos, fontSize, ImColor(1.f, 1.f, 1.f, color.Value.w * AnimationPerc), true);

        ImGui::PopFont();
    }
};



void Regen::OnRenderProgressZephyr()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    float lastAnimationPerc = AnimationPerc;
    static float lastProgress = 0.f;
    if (!RenderProgress.Enabled)
    {
        return;
    }
    // Get the middle of the screen
    ImVec2 pos = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2.5f);
    pos.y += pos.y / 2;
    pos.y += ProgressYOffset.Value;



    if (AnimationPerc > 0.99f) AnimationPerc = 1.f;

    // Render the progress text
    bool isRedstone = false;

    if (CurrentBlock.block && CurrentBlock.block->blockLegacy) isRedstone = CurrentBlock.block->blockLegacy->id == 73 || CurrentBlock.block->blockLegacy->id == 74;

#ifndef __RELEASEFREE__
    bool isQueuedOre = ShouldQueueOres() && isRedstone && player->getAbsorption() >= 10.f && AbsorptionCheck.Enabled;
#else
    bool isQueuedOre = ShouldQueueOres() && isRedstone && player->getAbsorption() >= 10.f;
#endif
    bool isUncoveringRedstone = !isRedstone;
    float progress = 1.f;

    progress = Progress;
    // Lerp the progress with lastProgress and use Minecraft::ClientInstance->GetDeltaTime() to make it smooth
    progress = Math::Lerp(lastProgress, progress, ImGui::GetIO().DeltaTime * 30.f);
    lastProgress = progress;

    // Clamp the progress to 0-1
    progress = std::clamp(progress, 0.f, 1.f);

    if (progress > 0.99f) progress = 1.f;
    float absoprtion = player->getAbsorption();

    std::string abs = "(" + std::to_string((int)absoprtion) + "/10)";

    static std::string text = "Mining redstone";

    static ImColor oldColor = color;
    float lerpSpeed = 12.f;


    float sped = ImGui::GetIO().DeltaTime * lerpSpeed;

#ifndef __RELEASEFREE__
    if (isQueuedOre && Enabled && IsStealing && isRedstone && IsBreaking) {
        color = ImColor(235, 110, 52, 255);
        text = xorstr_("Stealing redstone");

        AnimationPerc = Math::Lerp(AnimationPerc, 1.f, sped);
    } if (isQueuedOre && Enabled && IsBreaking)
    {
        color = ImColor(0, 255, 255, 255);
        if (progress < 1.f) text = xorstr_("Queueing ore");
        else text = xorstr_("Queued ore");

        AnimationPerc = Math::Lerp(AnimationPerc, 1.f, sped);
    } else
#endif
        if (isUncoveringRedstone && Enabled && IsBreaking)
        {
            color = ImColor(255, 0, 0, 255);
            text = xorstr_("Uncovering redstone");

            AnimationPerc = Math::Lerp(AnimationPerc, 1.f, sped);
        }
#ifndef __RELEASEFREE__
        else if (Enabled && isRedstone && IsBreaking && IsStealing)
        {
            color = ImColor(235, 110, 52, 255);
            text = xorstr_("Stealing redstone");

            AnimationPerc = Math::Lerp(AnimationPerc, 1.f, sped);
        }
#endif
        else if (IsBreaking && isRedstone && Enabled)
        {
            color = ImColor(0, 255, 0, 255);
            text = "Mining redstone " + abs;

            AnimationPerc = Math::Lerp(AnimationPerc, 1.f, sped);
        } else
        {
            AnimationPerc = Math::Lerp(AnimationPerc, 0.f, sped);
        }

    if (AnimationPerc <= 0.01f || text.empty()) return;
    float colorLerpSpeed = 7.5f;

    float fontSize = ProgressSize.Value * AnimationPerc;

    std::string displayText = text;

    ImGui::PushFont(Font::Fonts["greycliff_demibold_large"]);

    ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, displayText.c_str());

    // center the pos
    pos.x -= textSize.x / 2;

    ImGui::GetBackgroundDrawList()->PushClipRect(ImVec2(pos.x + (textSize.x * progress), pos.y), ImVec2(pos.x + textSize.x, pos.y + textSize.y), true);
    Render::RenderText(displayText, pos, fontSize, ImColor(1.f, 1.f, 1.f, color.Value.w * AnimationPerc), true);
    ImGui::GetBackgroundDrawList()->PopClipRect();

    ImGui::GetBackgroundDrawList()->PushClipRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + textSize.x * progress, pos.y + textSize.y), true);
    for (int i = 0; i < displayText.size(); i++)
    {
        ImColor color = HUD::GetColor(i);
        ImVec2 ptextSize = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0, std::string(1, displayText[i]).c_str());

        Render::RenderText(std::string(1, displayText[i]), pos, fontSize, ImColor(color.Value.x, color.Value.y, color.Value.z, color.Value.w * AnimationPerc), true);
        pos.x += ptextSize.x;
    }
    ImGui::GetBackgroundDrawList()->PopClipRect();

    ImGui::PopFont();
}

void Regen::OnRender()
{
    if (!Minecraft::ClientInstance->levelRenderer)
    {
        Reset();
        CurrentBlock = BlockPosPair(Vector3(), nullptr);
        return;
    }
    if (Enabled) OnRenderBlock();
    switch ((enum ProgressStyle)ProgressStyle.SelectedIndex)
    {
    case ProgressStyle::Default:
        OnRenderProgress();
        break;
    case ProgressStyle::Zephyr:
        OnRenderProgressZephyr();
        break;
    }
}

bool Regen::FindFunnyBlock(std::vector<BlockPosPair> blocks, BlockPosPair &pair, float v1, float v2) {

#ifndef __RELEASEFREE__
    std::map<Vector3, float> blockSpeeds = std::map<Vector3, float>();

    for (BlockPosPair& block : blocks) {
        // Check if the block id is 73 or 74
        if (bool isRedstone = block.block->blockLegacy->id == 73 || block.block->blockLegacy->id == 74; !isRedstone) continue;
        if (block.GetAABB().GetClosestPoint(*Minecraft::ClientInstance->ThePlayer()->getPos()).Distance(*Minecraft::ClientInstance->ThePlayer()->getPos()) > v1) continue;


        if (FunnyMode == (int)FunnyMode::AllSides) {
            // Get blocks surrounding this redstone with the second param turned to TRUE
            std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(block.blockPos, true);

            for (auto &blockAbove: surroundingBlocks) {
                if (blockAbove.second.block->blockLegacy->id == 73 ||
                    blockAbove.second.block->blockLegacy->id == 74)
                    continue;
                if (blockAbove.second.GetAABB().GetClosestPoint(
                        *Minecraft::ClientInstance->ThePlayer()->getPos()).Distance(
                        *Minecraft::ClientInstance->ThePlayer()->getPos()) > v2)
                    continue;

                if (IsExposed(blockAbove.second.blockPos)) {
                    blockSpeeds[blockAbove.second.blockPos] = Misc::GetDestroySpeed(
                            Misc::GetBestBreakingTool(blockAbove.second.block), blockAbove.second.block, 1.f);
                }
            }
        } else if (FunnyMode == (int)FunnyMode::TopOnly) {
            // Get the block 2 blocks above this redstone
            BlockPosPair blockAbove = BlockPosPair(block.blockPos + Vector3(0, 2, 0), Minecraft::ClientInstance->GetBlockSource()->getBlock(block.blockPos + Vector3(0, 2, 0)));
            if (blockAbove.block->blockLegacy->id == 0) continue;

            std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(blockAbove.blockPos, false);
            bool foundRedstone = false;
            for (auto& block : surroundingBlocks) {
                if (block.second.block->blockLegacy->id == 73 || block.second.block->blockLegacy->id == 74) {
                    foundRedstone = true;
                    break;
                }
            }

            if (!foundRedstone) {
                blockSpeeds[blockAbove.blockPos] = Misc::GetDestroySpeed(Misc::GetBestBreakingTool(blockAbove.block), blockAbove.block, 1.f);
            }
        } else if (FunnyMode == (int)FunnyMode::RightSide) {
            BlockPosPair blockRight = BlockPosPair(block.blockPos + Vector3(2, 0, 0), Minecraft::ClientInstance->GetBlockSource()->getBlock(block.blockPos + Vector3(2, 0, 0)));
            if (blockRight.block->blockLegacy->id == 0) continue;

            std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(blockRight.blockPos, false);
            bool foundRedstone = false;
            for (auto& block : surroundingBlocks) {
                if (block.second.block->blockLegacy->id == 73 || block.second.block->blockLegacy->id == 74) {
                    foundRedstone = true;
                    break;
                }
            }

            if (!foundRedstone) {
                blockSpeeds[blockRight.blockPos] = Misc::GetDestroySpeed(Misc::GetBestBreakingTool(blockRight.block), blockRight.block, 1.f);
            }
        }
    }

    // Sort the map by the destroy speed (highest first)
    std::vector<std::pair<Vector3, float>> sorted(blockSpeeds.begin(), blockSpeeds.end());

    if (UncoverSorting.SelectedIndex == (int)UncoverSorting::DestroySpeed) {
        std::sort(sorted.begin(), sorted.end(),
                  [](const std::pair<Vector3, float> &a, const std::pair<Vector3, float> &b) {
                      return a.second > b.second;
                  });
    } else if (UncoverSorting.SelectedIndex == (int)UncoverSorting::Distance) {
        std::sort(sorted.begin(), sorted.end(),
                  [](std::pair<Vector3, float> &a, std::pair<Vector3, float> &b) {
                        return a.first.Distance(*Minecraft::ClientInstance->ThePlayer()->getPos()) < b.first.Distance(*Minecraft::ClientInstance->ThePlayer()->getPos());
                    });
    }

    // if the map isnt empty then return the first element
    if (!sorted.empty()) {
        pair = BlockPosPair(sorted[0].first, Minecraft::ClientInstance->GetBlockSource()->getBlock(sorted[0].first));
        return true;
    }

#endif
    return false;

}
