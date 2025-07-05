//
// Created by vastrakai on 12/21/2023.
//

#include "PacketMine.h"

#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/SDK/Game/Misc/Structs.h>
#include <Prax/UI/ThemeManager.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/ColorUtil.h>
#include <Prax/SDK/Game/Network/Other/InventoryTransactionPacket.h>
#include <Prax/SDK/Game/Network/Other/PlayerActionPacket.h>

// position, side, progress

std::vector<QueuedBlockData> PacketMine::BlocksToMine = {};
bool PacketMine::IsDestroying = false;
float PacketMine::BreakProgress = 0.f;
bool PacketMine::Rotate = false;
Vector3 PacketMine::LastPos = Vector3();


void PacketMine::OnDisable()
{
    BlocksToMine.clear();
    IsDestroying = false;
    BreakProgress = 0.f;
    Rotate = false;
    LastPos = Vector3();
}

void PacketMine::RotateIf(Vector3 block, int side)
{
    if (RotateSetting.Enabled)
    {
        Rotate = true;
        LastPos = block;
    }
}

std::map<uint64_t, PacketMine::QueuedPacket> PacketMine::QueuedPackets = {};

void PacketMine::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

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
        }
    }

    if (BlocksToMine.empty())
    {
        IsDestroying = false;
        return;
    }

    // Remove blocks that dont have an exposed side
    for (auto it = BlocksToMine.begin(); it != BlocksToMine.end();)
    {
        if (Misc::GetExposedBlockFace(it->Position) == -1 && ExposedOnly.Enabled)
        {
            it = BlocksToMine.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // make sure the current block is still
    if (BlocksToMine.empty())
    {
        IsDestroying = false;
        return;
    }

    std::vector<QueuedBlockData> newMap = {};
    for (auto queuedBlock : BlocksToMine)
    {


        // Check if the block is already in the new map
        bool found = false;
        for (auto& [newBlock, newSide, blockPtr] : newMap)
        {
            if (newBlock == queuedBlock.Position)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            newMap.push_back({ queuedBlock.Position, queuedBlock.Side });
        }
    }

    BlocksToMine = newMap;

    // Get the first block in the map
    auto queuedBlock = BlocksToMine.begin();


    if (Math::Distance(queuedBlock->Position, *player->getPos()) > 7.f)
    {
        BlocksToMine.erase(queuedBlock);
        return;
    }


    Block* blockPtr = Minecraft::ClientInstance->GetBlockSource()->getBlock(queuedBlock->Position);

    bool outBool = false;
    int slot = SpoofToBestTool.Enabled ? Misc::GetBestBreakingTool(blockPtr) : player->supplies->selectedHotbarSlot;
    float destroySpeed = Misc::GetDestroySpeed(slot, blockPtr, DestroySpeed.Value);

    if (destroySpeed == 0.f) destroySpeed = 0.2f;

    static uint64_t blockBreakTime = 0;

    if (!IsDestroying)
    {
        // Start destroying the block
        player->gamemode->_startDestroyBlock(queuedBlock->Position, queuedBlock->Side + 6, outBool);
        if (outBool)
        {
            RotateIf(queuedBlock->Position, queuedBlock->Side);
            return;
        }
        player->gamemode->_continueDestroyBlock(queuedBlock->Position, queuedBlock->Side + 6, *player->getPos(), outBool);
        IsDestroying = true;
        blockBreakTime = Misc::GetBlockBreakTime(destroySpeed);
    } else
    {
        if (Minecraft::ClientInstance->GetTimer() == 20 && Minecraft::ClientInstance->GetTimerMultiplier() == 1.f)
            BreakProgress += destroySpeed;
        else
            BreakProgress = (blockBreakTime - Misc::GetCurrentMs() + 3) / blockBreakTime;

        if (BreakProgress + destroySpeed >= 1.f)
            BreakProgress = 1.f;
    }

    // Stop mining if the block is air
    if (Misc::IsAirBlock(queuedBlock->Position))
    {
        IsDestroying = false;
        BreakProgress = 0.f;
        BlocksToMine.erase(queuedBlock);
        return;
    }

    if (BreakProgress >= 1.f)
    {
        // Finish destroying the block
        bool oldIsDestroying = player->isDestroying;
        player->isDestroying = true;

        player->gamemode->breakDuration = 1.f;
        int oldSlot = player->supplies->selectedHotbarSlot;
        player->SelectSlot(slot);

        auto pkt = MinecraftPackets::CreatePacket<InventoryTransactionPacket>();

        auto cit = std::make_unique<ItemUseInventoryTransaction>();
        cit->actionType = ItemUseInventoryTransaction::ActionType::Destroy;
        cit->slot = slot;
        cit->itemInHand = *player->supplies->inventory->getItem(slot);
        cit->blockPos = queuedBlock->Position;
        cit->face = queuedBlock->Side;
        cit->targetBlockRuntimeId = 0; // Infinite durability?!?!?!
        cit->playerPos = *player->getPos();

        BlockPosPair block = BlockPosPair(queuedBlock->Position, Minecraft::ClientInstance->GetBlockSource()->getBlock(queuedBlock->Position));
        AABB blockAABB = block.GetAABB();

        cit->clickPos = blockAABB.GetClosestPoint(*player->getPos());
        pkt->mTransaction = std::move(cit);

        QueuedPackets[Misc::GetCurrentMs()] = { queuedBlock->Position, pkt, slot };

        //player->gamemode->_destroyBlock(queuedBlock->Position, queuedBlock->Side + 6);

        player->SelectSlot(oldSlot);
        player->gamemode->breakDuration = 0.f;
        player->isDestroying = oldIsDestroying;

        IsDestroying = false;
        BreakProgress = 0.f;

        // Remove the block from the map
        BlocksToMine.erase(queuedBlock);

        RotateIf(queuedBlock->Position, queuedBlock->Side);
    }
}

void PacketMine::OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    if (packet->getId() == (int)PacketID::PlayerAuthInput)
    {
        auto* paip = (PlayerAuthInputPacket*)packet;

        if (Rotate) {
            if (Math::IsPosWithinAngle(30.f, LastPos))
            {
                Rotate = false;
                return;
            }

            Vector2 rotations = Misc::GetRotations(player->getPos(), &LastPos);

            paip->rotation = rotations;
            paip->headYaw = rotations.y;
            Rotate = false;
        }

        if (paip->playerBlockActions.mActions.empty()) return;

        std::vector<PlayerBlockActionData> newActions = {};

        for (auto action : paip->playerBlockActions.mActions)
        {
            // If this is a start mining packet, add the block to the list

            bool isQueuedTransac = action.facing > 5 || action.facing < 0;
            if (isQueuedTransac)
            {
                if (action.facing > 5) action.facing -= 6;
                else if (action.facing < 0) action.facing += 6;
                newActions.push_back(action);
                continue;
            }

            Block* blockPtr = Minecraft::ClientInstance->GetBlockSource()->getBlock(action.position);

            if (OresOnly.Enabled && !Misc::IsOreBlock(blockPtr)) continue;

            if (action.action == PlayerActionType::START_DESTROY_BLOCK)
            {
                // Add only if the pos is not already in the list

                BlocksToMine.push_back({ action.position, action.facing, blockPtr });
            }
        }

        // Add the new actions to the packet
        paip->playerBlockActions.mActions = newActions;

    }
}
float lastProgress = 0.f;

void PacketMine::OnRender()
{
    if (!RenderQueued.Enabled) return;

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    int index = 0;

    Vector3 targetedBlock = BlocksToMine.empty() ? Vector3() : BlocksToMine.begin()->Position;

    for (auto queuedBlock : BlocksToMine)
    {
        float progress = 1.f;
        if (queuedBlock.Position == targetedBlock)
        {
	        progress = 1.f - BreakProgress;
            // Lerp the progress with lastProgress and use Minecraft::ClientInstance->GetDeltaTime() to make it smooth
            if (progress > lastProgress) lastProgress = progress;
            progress = Math::Lerp(lastProgress, progress, ImGui::GetIO().DeltaTime * 30.f);
            lastProgress = progress;
        }
        if (progress < 0.02f) continue;

        Vector3 pos = queuedBlock.Position;
        // make sure the pos is adjusted for progress so it's still centered (If the progress is 1, it will be centered)
        pos.x += 0.5f - (progress / 2.f);
        pos.y += 0.5f - (progress / 2.f);
        pos.z += 0.5f - (progress / 2.f);


        AABB aabb = AABB(pos, Vector3(progress, progress, progress));

        if (!ThemeManager::CurrentTheme)
            return;

        Math::DrawOutlinedBox(aabb.lower, aabb.upper, 1.0f, ThemeManager::CurrentTheme->primaryColor.color);
        index++;
    }
}
