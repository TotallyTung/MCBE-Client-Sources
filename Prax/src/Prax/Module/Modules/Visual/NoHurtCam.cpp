//
// Created by vastrakai on 12/28/2023.
//

#include "NoHurtCam.h"

void NoHurtCam::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    int* hurtTime = player->hurtTime;
    if (!hurtTime)
    {
        player->addChatMessage("hurtTime is null", 0);
        return;
    }

    *hurtTime = 0;
}

void NoHurtCam::OnRender()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    int* hurtTime = player->hurtTime;
    if (!hurtTime)
    {
        player->addChatMessage("hurtTime is null", 0);
        return;
    }

    *hurtTime = 0;
}