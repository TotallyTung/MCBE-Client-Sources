//
// Created by vastrakai on 4/14/2024.
//

#include <Prax/Module/Modules/Combat/Aura.h>
#include <imgui_internal.h>
#include "FollowBot.h"

void simulateForward() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    player->GetMoveInputComponent()->forward = true;
}

/*void simulateDirectionTowardsPos(Vector3 targetPos) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    Vector3 playerPos = player->GetStateVectorComponent()->pos;

    // Calc yaw towards target
    float yaw = atan2(targetPos.z - playerPos.z, targetPos.x - playerPos.x) * (180 / IM_PI) - 90;
    float yawDiff = player->GetActorRotationComponent()->yaw - yaw;

    // Depending on the yaw press one of the movement keys
    bool holdW = false;
    bool holdS = false;
    bool holdA = false;
    bool holdD = false;

    // Make sure to account for yaw wrapping around from -180 to 180
    if (yawDiff < -180) yawDiff += 360;
    if (yawDiff > 180) yawDiff -= 360;

    if (yawDiff > -45 && yawDiff < 45) holdW = true;
    if (yawDiff > 135 || yawDiff < -135) holdS = true;
    if (yawDiff > 45 && yawDiff < 135) holdA = true;
    if (yawDiff < -45 && yawDiff > -135) holdD = true;

    simulateDir(holdW, holdS, holdA, holdD);
}*/

void FollowBot::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;



    // Get the aura target
    if (!Aura::HasTarget) return;

    Actor* daTarget = Aura::LastTargetedActor;

    // Get the player's position
    Vector3 playerPos = player->GetStateVectorComponent()->pos;

    // Get the target's position
    Vector3 targetPos = daTarget->GetStateVectorComponent()->pos;

    // calculate rots to target
    float yaw = atan2(targetPos.z - playerPos.z, targetPos.x - playerPos.x) * (180 / IM_PI) - 90;
    float pitch = atan2(targetPos.y - playerPos.y, sqrt(pow(targetPos.x - playerPos.x, 2) + pow(targetPos.z - playerPos.z, 2))) * (180 / IM_PI);

    auto rotComponent = player->GetActorRotationComponent();

    Vector2 pAngles = Vector2(rotComponent->pitch, rotComponent->yaw);

    Vector2 delta = Vector2(-(pitch - pAngles.x), yaw - pAngles.y);

    delta.x = 0.f;

    // if the player is within 2 blocks of the target, return
    if (playerPos.Distance(targetPos) < 2) return;

    player->applyTurnDelta(delta);


    // if the player is within 2 blocks of the target, return
    if (playerPos.Distance(targetPos) < 2) return;

    if (player->isCollidingHorizontal()) player->jumpFromGround();
    simulateForward();
}

void FollowBot::OnRender() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    // Get the aura target
    if (!Aura::HasTarget) return;


}

void FollowBot::OnDisable() {
}
