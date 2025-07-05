//
// Created by vastrakai on 4/15/2024.
//

#include <imgui_internal.h>
#include <Prax/Module/Modules/Combat/Aura.h>
#include <Prax/Module/Modules/Movement/Speed.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/World/Scaffold.h>
#include "TargetStrafe.h"

void simulateDir(bool forward, bool backward, bool left, bool right) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (forward) player->GetMoveInputComponent()->forward = forward;
    if (backward) player->GetMoveInputComponent()->backward = backward;
    if (left) player->GetMoveInputComponent()->left = left;
    if (right) player->GetMoveInputComponent()->right = right;
}

void simulateDirectionTowardsPos(Vector3 targetPos) {
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
}

#define NOW Misc::GetCurrentMs()

void TargetStrafe::OnTick() {
    static auto speed = ModuleManager::GetModule<Speed>();

    if (!SpeedOnly.Enabled) {
        strafe();
    }
}

void TargetStrafe::strafe() const {
    static auto scaffold = ModuleManager::GetModule<Scaffold>();

    Aura::ServerSide.Enabled = true;
    static float lastRange = 0;
    static bool wasWithinLastTick = false;
    static bool isStillAdjusting = false;

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) {
        lastRange = 0;
        wasWithinLastTick = false;
        isStillAdjusting = false;
        return;
    }

    if (scaffold->Enabled) {
        lastRange = 0;
        wasWithinLastTick = false;
        isStillAdjusting = false;
        return;
    }

    if (!Aura::HasTarget) {
        lastRange = 0;
        wasWithinLastTick = false;
        isStillAdjusting = false;
        return;
    }

    Actor* daTarget = Aura::LastTargetedActor;
    if (!daTarget) {
        lastRange = 0;
        wasWithinLastTick = false;
        isStillAdjusting = false;
        return;
    }

    Vector3 playerPos = player->GetStateVectorComponent()->pos;
    Vector3 targetPos = daTarget->GetStateVectorComponent()->pos;

    float distance = playerPos.Distance(targetPos);
    float radius = this->Radius.Value;

    // if the distance is less than the radius, keep strafing
    static bool direction = true; // true = right, false = left
    static uint64_t lastSwitch = 0;

    if (distance < this->Range.Value && (!this->RequireJump.Enabled || KeyManager::Keys[VK_SPACE])) {
        Aura::ServerSide.Enabled = false;

        if (SpeedOnly.Enabled) {
            // Set the isJumping flag to false so we don't double jump
            player->GetMoveInputComponent()->isJumping = false;
        }

        if (distance - 0.4 > radius) {
            isStillAdjusting = false;
            wasWithinLastTick = false;
            simulateDir(true, false, false, false);
        } else {
            if (lastRange == 0) {
                simulateDir(false, false, !direction, direction);
            } else {
                if (isStillAdjusting) {
                    auto isWithin = radius > distance;
                    if (isWithin) {
                        isStillAdjusting = false;
                    }
                    wasWithinLastTick = isWithin;
                    simulateDir(isStillAdjusting, false, !direction, direction);
                } else {
                    if (lastRange < distance) {
                        auto isWithin = radius > distance;
                        if (!isWithin) {
                            isStillAdjusting = true;
                        }
                        wasWithinLastTick = isWithin;
                        simulateDir(isStillAdjusting, false, !direction, direction);
                    } else {
                        bool backward = distance < this->SpacingRadius.Value;
                        if (backward) {
                            simulateDir(false, true, !direction, direction);
                        } else {
                            simulateDir(false, false, !direction, direction);
                        }
                    }
                }
            }
        }

        lastRange = distance;

        // switch directions if we collide with a block
        if (this->SwitchDirections.Enabled && NOW - lastSwitch > 100) {
            if (player->isCollidingHorizontal()) {
                direction = !direction;
                lastSwitch = NOW;
            }
        }
    }
}

void TargetStrafe::OnDisable() {
    Aura::ServerSide.Enabled = true;
    Module::OnDisable();
}
