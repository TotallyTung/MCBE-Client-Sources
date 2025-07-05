//
// Created by vastrakai on 9/19/2023.
//

#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/SDK/Game/Network/Other/MovePlayerPacket.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/SDK/Game/Network/Other/MobEquipmentPacket.h>
#include <Prax/Module/Modules/Visual/PlaceHighlights.h>
#include "AntiRegen.h"

#include <Prax/Hook/Hooks/RakPeerHooks.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/SDK/Game/Network/Other/LevelEventPacket.h>

#include "Regen.h"

#ifndef __RELEASEFREE__

std::vector<std::pair<BlockPosPair, uint64_t>> AntiRegen::BlockQueue = {};
std::vector<std::pair<uint64_t, Vector3>> AntiRegen::PlayerBlocks = {};
bool AntiRegen::ShouldRotate = false;
BlockPosPair AntiRegen::CurrentBlock = BlockPosPair(Vector3(0, 0, 0), nullptr);
uint64_t AntiRegen::LastPlaceMs = 0;

void AntiRegen::CoverBlock(BlockPosPair block)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    // Return if the block is in the PlayerBlocks
    for (auto playerBlock : PlayerBlocks)
    {
        if (playerBlock.second == block.blockPos) return;
    }

    Vector3 pos = block.blockPos;

    static Regen* regen = ModuleManager::GetModule<Regen>();

    if (player->getAbsorption() >= 10 && regen->Enabled && Regen::IsBreaking && Regen::CurrentBlock.blockPos == block.blockPos && IgnoreQueuedOres.Enabled) return;

    Vector3 closestPoint = block.GetAABB().GetClosestPoint(*player->getPos());
    float distance = closestPoint.Distance(*player->getPos());
    if (distance > Distance.Value) return;

    // Get the first exposed side
    int exposedSide = Misc::GetExposedBlockFace(pos);
    if (exposedSide == -1) return;

    if (Misc::IsAirBlock(pos)) return; // return if the original block is air

    Vector3 offsetPos = pos + Misc::GetSide(exposedSide);
    int placeFace = Misc::GetBlockPlaceFace(offsetPos);

    // if the placeFace is 0 (down), return (we will flag the anticheat)
    if (placeFace == 0) return;

    auto slot = Misc::GetPlaceableItemOnBlock(offsetPos, false, false);
    if (slot == -1) return;

    // Switch to the slot
    player->SelectSlot(slot);

    // Place the block
    LastPlaceMs = Misc::GetCurrentMs();
    Misc::PlaceBlock(offsetPos, placeFace, false);
}

uint64_t LastPlaceMs = 0;

void AntiRegen::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (!player) return;

    if (player->getAbsorption() >= 10 && AbsorptionCheck.Enabled) return;

    static Regen* regen = ModuleManager::GetModule<Regen>();

    if (regen->Enabled && Regen::ShouldRotate) return;

    if (Mode == COVER_ON_DESTROY)
    {
        if (IgnoreLocalBreak.Enabled)
        {
            // Remove air blocks from PlayerBlocks
            for (auto it = PlayerBlocks.begin(); it != PlayerBlocks.end();)
            {
                if (Misc::IsAirBlock(it->second))
                {
                    it = PlayerBlocks.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            // Remove old blocks from PlayerBlocks
            for (auto it = PlayerBlocks.begin(); it != PlayerBlocks.end();)
            {
                if (it->first + 1000 < Misc::GetCurrentMs())
                {
                    it = PlayerBlocks.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        if (!BlockQueue.empty())
        {
            auto block = BlockQueue.begin();
            uint64_t delay = (uint64_t)(Delay.Value * 1000);
            if (PingCompensation.Enabled)
            {
                delay -= (RakPeerHooks::LastPing);
            }

            if (block->second + delay < Misc::GetCurrentMs())
            {
                CurrentBlock = block->first;
                ShouldRotate = true;
                CoverBlock(block->first);
                BlockQueue.erase(block);
            }
        }

        return;
    }


    bool placedBlock = false;

    if (PlacesPerTick.Value >= 1.f)
        for (int i = 0; i < PlacesPerTick.Value; i++)
            placedBlock = Tick();
    else if (PlacesPerTick.Value > 0.f && PlacesPerTick.Value < 1.f) {
        // Utilize the LastPlaceMs for tick delay (Each tick is 50ms)
        uintptr_t msPerPlace = (uintptr_t)(50 * (1.f / PlacesPerTick.Value));
        if (LastPlaceMs == 0 || LastPlaceMs + msPerPlace < Misc::GetCurrentMs())
            placedBlock = Tick();
    }
}

bool AntiRegen::Tick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    Vector3 closestRedstoneBlock = Misc::GetClosestExposedRedstone(Distance.Value);
    if (closestRedstoneBlock == Vector3(FLT_MAX, FLT_MAX, FLT_MAX)) return false;

    int exposedSide = Misc::GetExposedBlockFace(closestRedstoneBlock);

    Vector3 offsetPos = closestRedstoneBlock + Misc::GetSide(exposedSide);
    int placeFace = Misc::GetBlockPlaceFace(offsetPos);

    auto slot = Misc::GetPlaceableItemOnBlock(offsetPos, false, false);
    if (slot == -1) return false;

    // Switch to the slot
    player->SelectSlot(slot);

    ShouldRotate = true;
    CurrentBlock = BlockPosPair(offsetPos, Minecraft::ClientInstance->GetBlockSource()->getBlock(offsetPos));

    Misc::PlaceBlock(offsetPos, placeFace, false);

    return true;
}

void AntiRegen::OnEnable()
{

}

void AntiRegen::OnDisable()
{
    // Clear everything
    BlockQueue.clear();
    PlayerBlocks.clear();
    ShouldRotate = false;
    CurrentBlock = BlockPosPair(Vector3(0, 0, 0), nullptr);
}

void AntiRegen::OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (packet->getId() == (int)PacketID::PlayerAuthInput)
    {
        auto paip = (PlayerAuthInputPacket*)(packet);
        if (ShouldRotate)
        {
            // Rotate to closest point
            Vector2 rot = Misc::GetRotations(player->getPos(), &CurrentBlock.blockPos);
            rot.x = fmax(82, rot.x);
            paip->rotation = rot;
            paip->headYaw = rot.y;
            ShouldRotate = false;
        }


        if (Mode != COVER_ON_DESTROY) return;

        // Remove blocks from queue if we are mining
        if (!paip->playerBlockActions.mActions.empty() && IgnoreLocalBreak.Enabled)
        {
            for (auto action : paip->playerBlockActions.mActions)
            {
                // Add to PlayerBlocks
                PlayerBlocks.emplace_back(Misc::GetCurrentMs(), action.position);

                // If we are queueing a break, remove the block from the queue
                Vector3 actionPos = action.position;

                for (auto it = BlockQueue.begin(); it != BlockQueue.end();)
                {
                    if (it->first.blockPos == actionPos)
                    {
                        it = BlockQueue.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }

                // Reset the CurrentBlock if we are breaking it
                if (CurrentBlock.blockPos == actionPos)
                {
                    CurrentBlock = BlockPosPair(Vector3(0, 0, 0), nullptr);
                }
            }
        }
    }
};

void AntiRegen::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel)
{
    if (Mode != COVER_ON_DESTROY) return;
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    if (player->getAbsorption() >= 10 && AbsorptionCheck.Enabled) return;

    if (packet->getId() == (int)PacketID::LevelEvent)
    {
        auto lep = std::reinterpret_pointer_cast<LevelEventPacket>(packet);
        if (lep->eventId == 3600)
        {
            Vector3 pos = lep->pos;

            Vector3 closestPoint = AABB(pos, Vector3(1, 1, 1)).GetClosestPoint(*player->getPos());
            float distance = closestPoint.Distance(*player->getPos());
            if (distance > Distance.Value) return;

            Block* block = Minecraft::ClientInstance->GetBlockSource()->getBlock(pos);
            if (!block) return;

            if (block->blockLegacy->id == 73 || block->blockLegacy->id == 74 && RedstoneOnly.Enabled)
            {
                // Add to queue
                BlockQueue.emplace_back(BlockPosPair(pos, block), Misc::GetCurrentMs());
            } else if (!RedstoneOnly.Enabled)
            {
                // Add to queue
                BlockQueue.emplace_back(BlockPosPair(pos, block), Misc::GetCurrentMs());
            }
        }
    }
}

void AntiRegen::OnRender()
{

}

#endif