//
// Created by vastrakai on 9/23/2023.
//

#include <imgui_internal.h>
#include <Prax/Utils/Math.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/SDK/Game/Network/Other/MovePlayerPacket.h>
#include "AutoLootbox.h"

Vector2 AutoLootbox::Rotation = Vector2(0, 0);
uint64_t AutoLootbox::LastHitMs = 0;



void AutoLootbox::RotateToEntity(Actor* actor){
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    Vector3 pos = Misc::GetClosestAabbPos(actor, player->getPos());
    if (pos.Distance(*player->getPos()) < 2.2f) pos = Misc::GetLerpedActorPos(actor) + Vector3(0.f, 1.62f, 0.f);
    Vector2 rotations = Misc::GetRotations(player->getPos(), &pos);
    Rotation = rotations;
}

void AutoLootbox::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto actorList = player->level->getRuntimeActorList();
    // selectedActor->GetEntityId();

    static std::vector<uint64_t> lootedCrates;

    // Continue if the last hit was less than 200ms ago
    if (Misc::GetCurrentMs() - LastHitMs < 200) return;

    // Remove all actors with that dont have the id of "hivesky:death_crate"
    std::erase_if(actorList, [](auto actor) {
        return actor->GetEntityId() != "hivesky:death_crate";
    });

    // Remove actors more than 5 blocks away from the player
    std::erase_if(actorList, [](auto actor) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        auto playerPos = player->getPos();
        auto actorPos = actor->getPos();
        return playerPos->Distance(*actorPos) > 4;
    });

    // If there are no actors left, return
    if (actorList.empty()) return;

    // Get the closest actor
    Actor* closestActor = nullptr;


    // Loop through all actors and find the closest one
    for (auto actor : actorList) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        auto playerPos = player->getPos();
        auto actorPos = actor->getPos();
        if (playerPos->Distance(*actorPos) < 5) {
            closestActor = actor;
        }
    }

    if (!closestActor) return;
    if (std::find(lootedCrates.begin(), lootedCrates.end(), closestActor->GetRuntimeID()) != lootedCrates.end()) return;

    // attack the closest actor
    LastHitMs = Misc::GetCurrentMs();
    Rotation = Misc::GetRotations(player->getPos(), closestActor->getPos());

    if (Mode == (int)Mode::Break)
    {
        player->swing();
        player->gamemode->attack(closestActor);
    } else if (Mode == (int)Mode::Open)
    {
        player->swing();
        player->gamemode->interact(closestActor, *closestActor->getPos());
        lootedCrates.push_back(closestActor->GetRuntimeID());
    }
}

void AutoLootbox::OnPacket(class LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto paip = reinterpret_cast<PlayerAuthInputPacket*>(packet);
        // Rotate if the last hit was less than 200ms ago
        if (Misc::GetCurrentMs() - LastHitMs < 200) {
            paip->rotation = Rotation;
            paip->headYaw = Rotation.y;
        }
    }
    else if (packet->getId() == (int)PacketID::MovePlayer) {
        auto mp = reinterpret_cast<MovePlayerPacket*>(packet);
        // Rotate if the last hit was less than 200ms ago
        if (Misc::GetCurrentMs() - LastHitMs < 200) {
            mp->rotation = Rotation;
            mp->headYaw = Rotation.y;
        }
    }
}