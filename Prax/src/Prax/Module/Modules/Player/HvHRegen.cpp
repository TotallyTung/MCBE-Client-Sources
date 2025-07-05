//
// Created by vastrakai on 3/1/2024.
//

#include "HvHRegen.h"

#define NOW Misc::GetCurrentMs()

void HvHRegen::OnTick()
{
    static uint64_t lastBreak = 0;

    if (NOW - lastBreak < Delay.Value * 1000) return;

    auto player = Minecraft::ClientInstance->ThePlayer();
    float absorption = player->getAbsorption();
    if (absorption >= 10.f) return;

    std::vector<BlockPosPair> blocks = Misc::GetBlocksWithinDistance2(player->getPos(), Distance.Value);

    Vector3 targetBlock = Vector3(0, 0, 0);


    bool outBool = false;

    for (auto block : blocks)
    {
        if (absorption >= 10.f) break;

        bool isRedstone = block.block->blockLegacy->id == 73 || block.block->blockLegacy->id == 74;
        if (!isRedstone) continue;
        if (block.GetAABB().GetClosestPoint(*player->getPos()).Distance(*player->getPos()) > Distance.Value) continue;

        if (Mode.SelectedIndex == (int)RegenMode::Single)
        {
            targetBlock = block.blockPos;
            break;
        }
        if (Mode.SelectedIndex == (int)RegenMode::Multi)
        {
            player->gamemode->startDestroyBlock(block.blockPos, 1, outBool);
            bool oldSwing = player->isSwinging;
            int oldSwimgTime = player->swingProgress;
            player->gamemode->destroyBlock(block.blockPos, 1);
            if (!oldSwing) player->isSwinging = false;
            if (oldSwing) player->swingProgress = oldSwimgTime;
            absorption += 2.f; // 1 heart per ore mined
            lastBreak = NOW;
        }
    }

    if (targetBlock == Vector3(0, 0, 0)) return;

    player->gamemode->startDestroyBlock(targetBlock, 1, outBool);
    bool oldSwing = player->isSwinging;
    int oldSwimgTime = player->swingProgress;
    player->gamemode->destroyBlock(targetBlock, 1);
    if (!oldSwing) player->isSwinging = false;
    if (oldSwing) player->swingProgress = oldSwimgTime;
    lastBreak = NOW;
}
