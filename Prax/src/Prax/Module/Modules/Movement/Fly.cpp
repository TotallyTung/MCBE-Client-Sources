#pragma once

#include "Fly.h"

#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/D2D.h>
#include <Prax/SDK/Game/Network/Network.h>
#include <Prax/Utils/System/KeyManager.h>
#include <Prax/SDK/Game/Internal/MinecraftGame.h>

Vector3 aabbMin = Vector3(0.3f, 0.3f, 0.3f);
Vector3 aabbMax = Vector3(0.6f, 1.8f, 0.6f);
void Fly::OnEnable() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    aabbMin = Vector3(player->GetAABBShapeComponent()->AAX, player->GetAABBShapeComponent()->AAY, player->GetAABBShapeComponent()->AAZ);
    aabbMax = Vector3(player->GetAABBShapeComponent()->BBX, player->GetAABBShapeComponent()->BBY, player->GetAABBShapeComponent()->BBZ);
}

int Fly::Ticks = 19;
float speedc = 0;
float glides = 0;

void Fly::OnTick() {
    Ticks++;
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (FlyModeSetting.SelectedIndex == 1) {
        // Check if Ticks is a multiple of FrictionTicks
        if (Ticks % (int) FrictionTicks.Value == 0) {

            speedc = SpeedSetting.Value;
            glides = GlideSpeed.Value / 10 / 10;

        } else {
            speedc *= SpeedFrictionMultiplier.Value;
            glides *= GlideFrictionMultiplier.Value;
        }
    } else {
        speedc = SpeedSetting.Value;
        glides = GlideSpeed.Value / 10 / 10;
    }

    if (TimerBoost.Value > 0) Minecraft::ClientInstance->SetTimer(TimerBoost.Value);

    if (!player) return;

    Vector2 motion = Misc::GetMotion(player->GetActorRotationComponent()->yaw, speedc);
    bool space = KeyManager::Keys[VK_SPACE];
    bool shift = KeyManager::Keys[VK_SHIFT];

    float y;


    y = glides;
    if (!Glide.Enabled) {
        y = player->GetStateVectorComponent()->velocity.y;
        if (Fly::Ticks % (int)FrictionTicks.Value == 0) {
            y = glides;
        }
    }


    if (!Minecraft::ClientInstance->mcGame->canUseKeys()) {
        motion = Vector2(0.f, 0.f);
        player->GetStateVectorComponent()->velocity = Vector3(0.f, y, 0.f);
    } else {
        if (space) {
            y = SpeedSetting.Value / 10;
        } else if (shift) {
            y = -SpeedSetting.Value / 10;
        } else {
            y = glides;
        }

        if (abs(player->GetAABBShapeComponent()->AAY - aabbMin.y) > DistanceSetting.Value && DistanceSetting.Value > 0) {
            if (ResetTicksOnClip.Enabled && !Glide.Enabled) {
                Fly::Ticks = 0;
                speedc = SpeedSetting.Value;
                glides = GlideSpeed.Value / 10 / 10;
            }
            player->GetAABBShapeComponent()->AAY = aabbMin.y;
            player->GetAABBShapeComponent()->BBY = aabbMax.y;
        }


        if (!Glide.Enabled) {
            y = player->GetStateVectorComponent()->velocity.y;
            if (Fly::Ticks % (int)FrictionTicks.Value == 0) {
                y = glides;
            }
        }


        player->GetStateVectorComponent()->velocity = Vector3(motion.x, y, motion.y);
    }


}

void Fly::OnDisable() {
    Minecraft::ClientInstance->SetTimer(20.f);
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
}


