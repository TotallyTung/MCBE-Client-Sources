#pragma once

#include "AimAssist.h"

#include "Antibot.h"
#include <Prax/Utils/Math.h>
#include "../../../UI/ClickGui.h"
#include <Prax/Utils/Render/D2D.h>
#include <Prax/Module/Modules/Player/Friends.h>
#include "imgui_internal.h"
#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/Utils/Render/ColorUtil.h>
#include <Prax/Utils/Render/Render.h>

Vector2 pAngles = Vector2(0.f, 0.f);
Actor *aimingAt = nullptr;
__int64 lastSwitch = 0;

void AimAssist::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    std::vector<Actor *> actors = Antibot::GetActors();

    actors.erase(std::ranges::remove_if(actors, [&](Actor *a) {
        return a == player;
    }).begin(), actors.end());

    std::sort(actors.begin(), actors.end(), [&](Actor *a, Actor *b) {
        /*AABB aabb_1 = a->GetLerpedAABB();
        ImVec4 pos2d_1 = Math::Get2dDrawPoints(aabb_1.lower, aabb_1.upper);

        AABB aabb_2 = b->GetLerpedAABB();
        ImVec4 pos2d_2 = Math::Get2dDrawPoints(aabb_2.lower, aabb_2.upper);*/
        return a->getPos()->Distance(*player->getPos()) < b->getPos()->Distance(
                *player->getPos()) /*&& Math::GetDistance(ImVec2(pos2d_1.x, pos2d_1.y), center) < Math::GetDistance(ImVec2(pos2d_2.x, pos2d_2.y), center)*/;
    });

    auto actor = actors[0];

    HitResult* result = player->level->getHitResult();
    if (result->type != RaycastTypes::ENTITY || result->entity.id != actor->entityCtx.entityId)
    {
        if (!IsValid(actor)) return;

        __int64 currentTime = Misc::GetCurrentMs();
        if (aimingAt && (currentTime - lastSwitch < 700)) {
            return;
        }

        aimingAt = actor;
        lastSwitch = currentTime;
    }
}

bool AimAssist::IsEntityInFov(float angle, Vector2 currentAngles, Vector2 targetAngles) {
    //angles in -180 to 180
    float yawDelta = AngleDifference(currentAngles.y, targetAngles.y);
    float pitchDelta = AngleDifference(currentAngles.x, targetAngles.x);

    float yawDeltaAbs = abs(yawDelta);
    float pitchDeltaAbs = abs(pitchDelta);

    if (yawDeltaAbs > angle || pitchDeltaAbs > angle) return false;

    return true;
}

void AimAssist::OnRender() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    auto rotComponent = player->GetActorRotationComponent();
    if (!rotComponent) return;

    pAngles = Vector2(rotComponent->pitch, rotComponent->yaw);

    if (!IsValid(aimingAt)) {
        aimingAt = nullptr;
        return;
    }

    HitResult* result = player->level->getHitResult();
    if (result->type == RaycastTypes::ENTITY && result->entity.id == aimingAt->entityCtx.entityId)
    {
        // Reset target
        aimingAt = nullptr;
        return;
    }

    ImVec2 center = ImGui::GetIO().DisplaySize;
    center.x /= 2;
    center.y /= 2;

    auto owo = Misc::GetLerpedActorPos(player);

    Vector3 closestPos = Misc::GetClosestAabbPos(aimingAt, &owo);
    Vector2 calcAngles = Misc::GetRotations(&owo, &closestPos);

    if (!IsEntityInFov(MinimumFOV.Value, pAngles, calcAngles)) return;

    Vector2 delta = Vector2(-(calcAngles.x - pAngles.x), calcAngles.y - pAngles.y);

    if (SmoothFactor.Value > 0.1f && delta.x != 0 && delta.y != 0) {
        delta.x /= SmoothFactor.Value;
        delta.y /= SmoothFactor.Value;
    }
    float deltaT = ImGui::GetIO().DeltaTime;
    player->applyTurnDelta(delta * deltaT);
}

bool AimAssist::IsValid(Actor *a) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    return a != nullptr && a->isAlive() && !Friends::IsFriend(a) &&
           Misc::GetClosestAabbPos(a, player->getPos()).Distance(*player->getPos()) < DistanceSetting.Value;
}