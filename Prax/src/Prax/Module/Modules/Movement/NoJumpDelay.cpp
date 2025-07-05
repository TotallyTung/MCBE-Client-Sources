//
// Created by vastrakai on 8/26/2023.
//

#include "NoJumpDelay.h"

void NoJumpDelay::OnEnable(){
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    player->GetJumpComponent()->noJumpDelay = true;
}

void NoJumpDelay::OnDisable(){
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    player->GetJumpComponent()->noJumpDelay = false;
}

void NoJumpDelay::OnTick(){
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    player->GetJumpComponent()->noJumpDelay = true;
}