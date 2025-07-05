//
// Created by vastrakai on 9/16/2023.
//

#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/SDK/Game/Network/Other/MovePlayerPacket.h>
#include <Prax/SDK/Game/Network/Other/MobEquipmentPacket.h>
#include <Prax/SDK/Game/World/Block.h>
#include <Prax/Utils/String.h>
#include <Prax/Module/Modules/Player/Teams.h>
#include "Breaker.h"

float Breaker::BreakProgress = 0.0f;
bool Breaker::IsBreaking = false;
bool Breaker::ShouldRotate = false;
Actor* Breaker::Target = nullptr;
BlockPosPair Breaker::CurrentBlock = BlockPosPair({}, nullptr);

float maxDistance = 6.0f;

void Breaker::Reset() {
    BreakProgress = 0.0f;
    IsBreaking = false;
    ShouldRotate = false;
    Target = nullptr;
    CurrentBlock = BlockPosPair({}, nullptr);
}

void Breaker::OnEnable() {

}

bool Breaker::IsValidActor()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (Target == nullptr) return false;

    auto actors = player->level->getRuntimeActorList();

    if (std::ranges::find(actors, Target) == actors.end()) {
        return false;
    }

    return true;
}

uint64_t lastSwitchMs = 0;


void Breaker::OnDisable() {
    if (!Prax::Initialized) return;
    Reset();
}

void Breaker::AddDebugMessage(const std::string& message) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
}

void Breaker::OnTick()
{
    if (TreasuresSetting.Enabled) TickTreasure();
    if (BedsSetting.Enabled) TickBed();
}

void Breaker::TickBed()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (CurrentBlock.block != nullptr && IsBreaking)
    {
        float distance = CurrentBlock.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos());
        if (distance > maxDistance)
        {
            AddDebugMessage("Distance too far, resetting");
            Reset();
            return;
        }

        ShouldRotate = true;

        if (BreakProgress < 1.f)
        {
            float destroySpeed = Misc::GetDestroySpeed(player->supplies->selectedHotbarSlot, CurrentBlock.block, 1.f);
            // Minimum speed should be 0.08
            if (destroySpeed < 0.08f) destroySpeed = 0.08f;
            BreakProgress += destroySpeed;
            AddDebugMessage("Breaking block " + std::to_string(BreakProgress));
            return;
        }

        if (BreakProgress >= 1.f)
        {
            // Finish mining the block
            ShouldRotate = true;

            AddDebugMessage("Breaking block " + std::to_string(BreakProgress) + " (finished)");

            player->gamemode->breakDuration = 1.f;
            player->gamemode->destroyBlock(CurrentBlock.blockPos, 0);
            player->gamemode->breakDuration = 0.f;

            IsBreaking = false;
            BreakProgress = 0.0f;
            return;
        }

        return;
    }

    std::vector<BlockPosPair> blocks = Misc::GetBlocksWithinDistance2(player->getPos(), Distance.Value);
    std::erase_if(blocks, [](BlockPosPair block) {
        return !block.block->blockLegacy->name.contains("bed");
    });

    // Sort by Distance
    std::ranges::sort(blocks, [&](BlockPosPair a, BlockPosPair b) {
        return player->getPos()->Distance(a.blockPos) < player->getPos()->Distance(b.blockPos);
    });

    if (blocks.empty())
    {
        Reset();
        return;
    }

    bool outBool = false;

    BlockPosPair targetBlock = blocks[0];


    ShouldRotate = true;
    CurrentBlock = targetBlock;

    AddDebugMessage("Breaking bed at " + std::to_string(targetBlock.blockPos.x) + " " + std::to_string(targetBlock.blockPos.y) + " " + std::to_string(targetBlock.blockPos.z));

    player->gamemode->startDestroyBlock(targetBlock.blockPos, Misc::GetExposedBlockFace(targetBlock.blockPos), outBool);
    if (outBool) return;
    player->gamemode->continueDestroyBlock(targetBlock.blockPos, Misc::GetExposedBlockFace(targetBlock.blockPos), *player->getPos(), outBool);
    if (outBool) return;


    IsBreaking = true;
    CurrentBlock = targetBlock;
}

void Breaker::TickTreasure() {
    maxDistance = Distance.Value;
    auto player = Minecraft::ClientInstance->ThePlayer();
    std::vector<Actor*> actors = player->level->getRuntimeActorList();

    std::erase_if(actors, [](auto actor) {
        return actor->GetEntityId() != "hivewars:treasure";
    });

    // Remove actors that are too far away
    std::erase_if(actors, [](Actor* actor) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        return actor->getPos()->Distance(*player->getPos()) > maxDistance;
    });

    // remove actors on the same team using Teams::IsOnTeam
    std::erase_if(actors, [](Actor* actor) {
        return Teams::IsOnTeam(actor);
    });

    if (actors.empty()) return;


    Actor* actor = actors[0];

    Vector3 actorPos = *actor->getPos();
    ShouldRotate = true;
    Target = actor;

    player->gamemode->interact(actor, actorPos);
    player->swing();
    player->gamemode->attack(actor);
}

void Breaker::OnPacket(class LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    int packetId = packet->getId();

    Vector2 Rotations;

    if (!ShouldRotate) return;


    if (TreasuresSetting.Enabled) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        if (IsValidActor()) {
            Rotations = Misc::GetRotations(player->getPos(), Target->getPos());
        } else
        {
            Target = nullptr;
        }
    }
    if (BedsSetting.Enabled && CurrentBlock.block != nullptr) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;

        AABB aabb = CurrentBlock.GetAABB();
        Rotations = Misc::GetRotations(*player->getPos(), aabb);

    }


    if (packetId == (int)PacketID::PlayerAuthInput) {
        auto paip = (PlayerAuthInputPacket*)packet;

        paip->rotation = Rotations;
        paip->headYaw = Rotations.y;

        ShouldRotate = false;
    } else if (packetId == (int)PacketID::MovePlayer) {
        auto mpp = (MovePlayerPacket*)packet;

        mpp->rotation = Rotations;
        mpp->headYaw = Rotations.y;
    }
}

