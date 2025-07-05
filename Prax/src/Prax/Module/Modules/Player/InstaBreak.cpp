//
// Created by Flash on 7/11/2023.
//

#include <Prax/Utils/Math.h>
#include "InstaBreak.h"

void InstaBreak::OnDisable(){
    if (Mode.SelectedIndex == 1)
        Minecraft::ClientInstance->ThePlayer()->gamemode->breakDuration = 0.f;
}

void InstaBreak::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    BlockPos selectedPos = player->level->getHitResult()->blockPos;

    if (Mode.SelectedIndex == 1)
        Minecraft::ClientInstance->ThePlayer()->gamemode->breakDuration = 1.f;
    if (Mode.SelectedIndex == 2){
        if (player->isDestroying)
        {
            bool outBool = false;
            for (int i = 0; i < 20; i++)
            {
                // This will cause you to go FLYING on servers that use movement prediction (e.x. BDS, local worlds)
                player->sendInput();
            }

        }
    }
}
