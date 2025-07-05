//
// Created by vastrakai on 4/17/2024.
//

#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/Render.h>
#include "OreMiner.h"

#include <Prax/Module/Modules/Player/Regen.h>
#include <Prax/SDK/Game/Network/Other/PlayerActionPacket.h>

bool OreMiner::IsBreaking = false;
float OreMiner::Progress = 0.f;
BlockPosPair OreMiner::CurrentBlock = BlockPosPair({}, nullptr);
bool OreMiner::ShouldRotate = false;
std::map<Vector3, uint64_t> OreMiner::PastBlocks = std::map<Vector3, uint64_t>();
std::map<uint64_t, OreMiner::QueuedPacket> OreMiner::QueuedPackets = {};

void OreMiner::Reset() {
    IsBreaking = false;
    Progress = 0.f;
}

std::vector<int> OreMiner::GetOreType() {
    // 56, 129, 14, 15, 21, 16, 416, 658, 659, 660, 662, 657, 656, 655, 661
    constexpr int REDSTONE_ORE = 73;
    constexpr int REDSTONE_ORE_LIT = 74;
    constexpr int DIAMOND_ORE = 56;
    constexpr int EMERALD_ORE = 129;
    constexpr int GOLD_ORE = 14;
    constexpr int IRON_ORE = 15;
    constexpr int LAPIS_ORE = 21;
    constexpr int COAL_ORE = 16;
    constexpr int BARRIER = 416;
    constexpr int DEEPSLATE_REDSTONE_ORE = 658;
    constexpr int DEEPSLATE_LIT_REDSTONE_ORE = 659;
    constexpr int DEEPSLATE_DIAMOND_ORE = 660;
    constexpr int DEEPSLATE_EMERALD_ORE = 662;
    constexpr int DEEPSLATE_GOLD_ORE = 657;
    constexpr int DEEPSLATE_IRON_ORE = 656;
    constexpr int DEEPSLATE_LAPIS_ORE = 655;
    constexpr int DEEPSLATE_COAL_ORE = 661;

    std::vector<int> ores;

    // for each ore make sure u push back their deepslate counterpart
    if (Diamond.Enabled) {
        ores.push_back(DIAMOND_ORE);
        ores.push_back(DEEPSLATE_DIAMOND_ORE);
    }
    if (Emerald.Enabled) {
        ores.push_back(EMERALD_ORE);
        ores.push_back(DEEPSLATE_EMERALD_ORE);
    }
    if (Gold.Enabled) {
        ores.push_back(GOLD_ORE);
        ores.push_back(DEEPSLATE_GOLD_ORE);
    }
    if (Iron.Enabled) {
        ores.push_back(IRON_ORE);
        ores.push_back(DEEPSLATE_IRON_ORE);
    }
    if (Lapis.Enabled) {
        ores.push_back(LAPIS_ORE);
        ores.push_back(DEEPSLATE_LAPIS_ORE);
    }
    if (Coal.Enabled) {
        ores.push_back(COAL_ORE);
        ores.push_back(DEEPSLATE_COAL_ORE);
    }
    if (AncientDebris.Enabled) {
        ores.push_back(450);
    }
    if (Copper.Enabled) {
        ores.push_back(451);
    }
    if (Amethyst.Enabled) {
        ores.push_back(452);
    }

    return ores;
}

bool OreMiner::IsExposed(Vector3 pos)
{
    std::map<int, BlockPosPair> surroundingBlocks = Misc::GetSurroundingBlocks(pos);

    bool isExposed = false;

    bool redstoneFound = false;

    for (auto& block : surroundingBlocks) {
        // Return true if the block is air
        if (block.second.block->blockLegacy->id == 0) return true;
        // Return true if the block is a chest
        if (block.second.block->blockLegacy->id == 54) return true;
        if (block.second.block->blockLegacy->id == 73 || block.second.block->blockLegacy->id == 74) redstoneFound = true;
    }

    // if the block at the pos is redstone here then return false
    auto block = Minecraft::ClientInstance->GetBlockSource()->getBlock(BlockPos(pos));
    if (block->blockLegacy->id == 73 || block->blockLegacy->id == 74) return false;
    if (redstoneFound) return false;

    return true;
}

void OreMiner::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (Regen::IsBreaking) return;

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
            ShouldRotate = true;
            Minecraft::ClientInstance->GetBlockSource()->clearBlock(packet.second.pos);
            QueuedPackets.erase(packet.first);
        }
    }

    // Remove recently mined blocks from most recent blocks
    for (auto it = PastBlocks.begin(); it != PastBlocks.end();) {
        if (Misc::GetCurrentMs() - it->second > RenderPastBlocksTime.Value * 1000) {
            it = PastBlocks.erase(it);
        } else {
            ++it;
        }
    }

    if (CurrentBlock.block && IsBreaking) {
        float destroySpeed = Misc::GetDestroySpeed(Misc::GetBestBreakingTool(CurrentBlock.block), CurrentBlock.block, DestroySpeed.Value);
        // If the block is water, set destroy speed to 0.20f
        if (String::ContainsIgnoreCase(CurrentBlock.block->blockLegacy->name, "water")) destroySpeed = 0.20f;
        if (String::ContainsIgnoreCase(CurrentBlock.block->blockLegacy->name, "lava")) destroySpeed = 0.20f;

        if (Progress < 1.f)
        {
            Progress += destroySpeed;
        }

        if (Progress < 1.f)
        {
            float destroySpeed = Misc::GetDestroySpeed(Misc::GetBestBreakingTool(CurrentBlock.block), CurrentBlock.block, DestroySpeed.Value);
            if (Progress + destroySpeed >= 1.f) Progress = 1.f; // Accounting for the delay that InfiniteDurability causes
        }

        bool finishBreaking = Progress >= 1.f;

        if (finishBreaking) {
            // Finish mining the block

            player->gamemode->breakDuration = 1.f;

            ShouldRotate = true;

            int slot = Misc::GetBestBreakingTool(CurrentBlock.block);

            auto pkt = MinecraftPackets::CreatePacket<InventoryTransactionPacket>();

            auto cit = std::make_unique<ItemUseInventoryTransaction>();
            cit->actionType = ItemUseInventoryTransaction::ActionType::Destroy;
            cit->slot = slot;
            cit->itemInHand = *player->supplies->inventory->getItem(slot);
            cit->blockPos = CurrentBlock.blockPos;
            cit->face = Misc::GetExposedBlockFace(CurrentBlock.blockPos);
            cit->targetBlockRuntimeId = 3690217760; // Infinite durability?!?!?!
            cit->playerPos = *player->getPos();
            Vector3 clickPos = CurrentBlock.blockPos;// - cit->playerPos;
            clickPos.x = cit->playerPos.x - clickPos.x;
            clickPos.y = cit->playerPos.y - clickPos.y;
            clickPos.z = cit->playerPos.z - clickPos.z;
            cit->clickPos = clickPos;

            pkt->mTransaction = std::move(cit);
            QueuedPackets[Misc::GetCurrentMs()] = { CurrentBlock.blockPos, pkt, slot };
            PastBlocks[CurrentBlock.blockPos] = Misc::GetCurrentMs();

            Reset();
            ShouldRotate = true;
            return;
        }

        return;
    }


    std::vector<int> ores = GetOreType();
    if (ores.empty()) return;

    std::vector<BlockPosPair> blocks = Misc::GetBlocksWithinDistance2(player->getPos(), Range.Value);

    BlockPosPair targetBlock = BlockPosPair({}, nullptr);

    for (auto& block : blocks) {
        if (std::find(ores.begin(), ores.end(), block.block->blockLegacy->id) != ores.end()) {
            if (IsExposed(block.blockPos)) {
                targetBlock = block;
                break;
            }
        }
    }

    if (!targetBlock.block) return;


    bool outBool = false;

    player->gamemode->startDestroyBlock(targetBlock.blockPos, 0, outBool);
    if (outBool) {
        ShouldRotate = true;
        CurrentBlock = targetBlock;
        return;
    }

    player->gamemode->continueDestroyBlock(targetBlock.blockPos, 0, *player->getPos(), outBool);
    if (outBool) {
        ShouldRotate = true;
        CurrentBlock = targetBlock;
        return;
    }

    CurrentBlock = targetBlock;
    IsBreaking = true;
}

void OreMiner::OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        auto* paip = (PlayerAuthInputPacket*) packet;

        if (ShouldRotate) {
            AABB aabb = CurrentBlock.GetAABB();

            Vector2 rotations = Misc::GetRotations(*player->getPos(), aabb);

            paip->rotation = rotations;
            paip->headYaw = rotations.y;

            ShouldRotate = false;
        }
    }
}

void OreMiner::OnDisable() {
    // clear everything
    Reset();
    CurrentBlock = BlockPosPair({}, nullptr);
    ShouldRotate = false;
    PastBlocks.clear();
}

void OreMiner::OnRender() {
    static float lastProgress = 0.f;
    if (RenderCurrentBlocks.Enabled) {
        if (CurrentBlock.block) {
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
                goto skip;
            }

            // make sure the pos is adjusted for progress so it's still centered (If the progress is 1, it will be centered)
            AABB fullAabb = AABB(targetedBlock, Vector3(1, 1, 1));
            targetedBlock.x += 0.5f - (progress / 2.f);
            targetedBlock.y += 0.5f - (progress / 2.f);
            targetedBlock.z += 0.5f - (progress / 2.f);

            AABB aabb = AABB(targetedBlock, Vector3(progress, progress, progress));


            Math::DrawBoxFilled(aabb.lower, aabb.upper, 1.0f, ImColor(255, 255, 255, 100), ImColor(255, 255, 255, 100));
        }
    }

    skip:

    if (RenderPastBlocks.Enabled) {
        for (auto& block : PastBlocks) {
            // fade the opacity from 100 to 0 over the time specified
            float time = (Misc::GetCurrentMs() - block.second) / 1000.f;
            int opacity = 100 - (time / RenderPastBlocksTime.Value) * 100;
            if (opacity <= 0) continue;

            Render::DrawBoxAroundBlock(block.first, ImColor(255, 255, 255, opacity), ImColor(255, 255, 255, opacity));

        }
    }
}
