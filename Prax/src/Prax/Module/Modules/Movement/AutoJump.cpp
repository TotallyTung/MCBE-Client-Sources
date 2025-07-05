//
// Created by vastrakai on 2/23/2024.
//

#include "AutoJump.h"

float AutoJump::EnableY = 0;
uint64_t AutoJump::LastJump = 0;

#define PLAYER_HEIGHT 1.62f;

void AutoJump::Jump()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    bool onGround = player->isOnGround();
    player->setOnGround(true);
    player->jumpFromGround();
    player->setOnGround(onGround);
};

void AutoJump::OnEnable() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    EnableY = player->getPos()->y;

    Minecraft::ClientInstance->SetTimer(TimerBoost.Value);


}

void AutoJump::OnDisable() {
    EnableY = 0;
    Minecraft::ClientInstance->SetTimer(20.f);
}



void AutoJump::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    Minecraft::ClientInstance->SetTimer(TimerBoost.Value);

    if (player->getPos()->y < EnableY && !player->isOnGround()) {
        if (Misc::GetCurrentMs() - LastJump > JumpDelay.Value * 1000) {
            Jump();
            LastJump = Misc::GetCurrentMs();
        }
        EnableY -= HeightLoss.Value;
    }

    if (player->isOnGround() && ResetOnGround.Enabled) {
        EnableY = player->getPos()->y;
    }
}

