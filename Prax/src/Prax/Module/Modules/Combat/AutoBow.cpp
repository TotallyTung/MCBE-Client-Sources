//
// Created by vastrakai on 10/10/2023.
//

#include "AutoBow.h"

#include <Prax/Hook/Hooks/RakPeerHooks.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Combat/Aura.h>
#include <Prax/Utils/Math.h>

#include "Antibot.h"

bool isChargingBow = false;
bool AutoBow::RotateNextTick = false;

void AutoBow::OnEnable() {
    //Misc::SetClipboard(String::ToHex((uintptr_t)Minecraft::ClientInstance));
    isChargingBow = false;
}




void AutoBow::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {

}



bool IsValidActor(Actor* actor)
{
    // Iterate thru the Antibot list and check if the actor is there
    for (auto a : Antibot::GetActors())
    {
        if (a == actor) return true;
    }
    return false;
}

void AutoBow::OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    if (AuraOnly.Enabled && !Aura::HasTarget) return;
    else if (!AuraOnly.Enabled) return;

    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto paip = (PlayerAuthInputPacket*)packet;
        if (!RotateNextTick) return;
        auto player = Minecraft::ClientInstance->ThePlayer();
        if (!player) return;
        if (!Aura::LastTargetedActor || !IsValidActor(Aura::LastTargetedActor)) {
            Aura::LastTargetedActor = nullptr;
            return;
        }

        // Check if the player has arrows
        bool found = false;
        for (int i = 0; i < 36; i++) {
            auto item = player->supplies->inventory->getItem(i);
            if (!item->item) continue;
            if ((*item->item)->id == 262) {
                found = true;
                break;
            }
        }

       if (!found) return;

        Actor* actor = Aura::LastTargetedActor;

        Vector3 pos = *actor->getPos();
        Vector3 posOld = *actor->getPosPrev();
        Vector3 posDelta = pos - posOld;

        Vector3 predPos = pos;
        float lastPing = RakPeerHooks::LastPing;

        float actorDistance = player->getPos()->Distance(pos);
        if (actorDistance < 1) actorDistance = 1;

        if (lastPing > 0) {
            predPos.x += posDelta.x * MoveCompensation.Value * (lastPing / 50.f);
            predPos.y += posDelta.y * MoveCompensation.Value * (lastPing / 50.f);
            predPos.z += posDelta.z * MoveCompensation.Value * (lastPing / 50.f);
        }
        predPos.y -= 0.5f;

        Vector2 rots = Misc::GetRotations(player->getPos(), &predPos);

        paip->rotation = rots;
        paip->headYaw = rots.y;
        RotateNextTick = false;
    }
}



void AutoBow::OnDisable() {
}

void AutoBow::OnRender()
{
/*#ifndef __RELEASEFREE__
    if (AuraOnly.Enabled && !Aura::HasTarget) return;
    else if (!AuraOnly.Enabled) return;

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    if (!Aura::LastTargetedActor || !IsValidActor(Aura::LastTargetedActor)) {
        Aura::LastTargetedActor = nullptr;
        return;
    }

    Actor* actor = Aura::LastTargetedActor;

    Vector3 pos = *actor->getPos();
    Vector3 posOld = *actor->getPosPrev();
    Vector3 posDelta = pos - posOld;

    Vector3 predPos = pos;
    float lastPing = RakPeerHooks::LastPing;

    float actorDistance = player->getPos()->Distance(pos);
    if (actorDistance < 1) actorDistance = 1;

    if (lastPing > 0) {

        //predPos += (posDelta * MoveCompensation.Value * (actorDistance)) * (lastPing / 50.f);
        predPos.x += posDelta.x * MoveCompensation.Value * (lastPing / 50.f);
        predPos.y += posDelta.y * MoveCompensation.Value * (lastPing / 50.f);
        predPos.z += posDelta.z * MoveCompensation.Value * (lastPing / 50.f);
    }
    predPos.y -= 1.62f;

    AABB aabb = actor->GetAABBForPos(predPos);

    Math::DrawOutlinedBox(aabb.lower, aabb.upper, 1.0f, ImColor(255, 0, 0, 255));
#endif*/
}


#define NOW Misc::GetCurrentMs()

void AutoBow::OnTick() {
    if (AuraOnly.Enabled && !Aura::HasTarget) return;

    static Aura* aura = ModuleManager::GetModule<Aura>();

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    // Get the aura target
    auto target = aura->LastTargetedActor;

    if (!target) return;
    if (!player->canSee(target)) return;

    int bowSlot = -1;

    for (int i = 0; i < 36; i++) {
        auto item = player->supplies->inventory->getItem(i);
        if (!item->item) continue;
        if ((*item->item)->GetName() == "bow") {
            bowSlot = i;
            break;
        }
    }

    if (bowSlot == -1) {
        return;
    }

    auto item = player->supplies->inventory->getItem(bowSlot);

    if (!item->item) return;

    static uint64_t start = 0;




    // If the bow is charging and its going to be released next tick, set RotateNetTick to true

    if (isChargingBow && NOW - start + 50 >= (Ticks.Value * 50)) {
        RotateNextTick = true;
    }

    if (!isChargingBow) {
        player->supplies->inventory->startUsingItem(bowSlot);
        isChargingBow = true;
        start = NOW;
    } else if (NOW - start >= (Ticks.Value * 50)) {
        player->supplies->inventory->releaseUsingItem(bowSlot);

        isChargingBow = false;
    }

}


