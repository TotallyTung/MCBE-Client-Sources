//
// Created by vastrakai on 8/30/2023.
//

#include <Prax/Module/ModuleManager.h>
#include "AutoTool.h"
#include "Regen.h"

int AutoTool::OldSlot = -1;

void AutoTool::OnDisable()
{
    if (OldSlot != -1)
    {
        auto player = Minecraft::ClientInstance->ThePlayer();
        player->SelectSlot(OldSlot);
        OldSlot = -1;
    }
}



void AutoTool::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    static Regen* regen = ModuleManager::GetModule<Regen>();
    bool isMining = player->isDestroying && !regen->IsBreaking;

    if (!isMining)
    {
        if (OldSlot != -1)
        {
            auto player = Minecraft::ClientInstance->ThePlayer();
            player->SelectSlot(OldSlot);
            OldSlot = -1;
        }
        return;
    }

    BlockSource* blockSource = Minecraft::ClientInstance->GetBlockSource();
    if (!blockSource) return;

    auto blockPos = player->level->getHitResult()->blockPos;


    auto block = blockSource->getBlock(blockPos);
    if (!block) return;

    int bestSlot = Misc::GetBestBreakingTool(block);

    if (bestSlot == -1) return;

    if (OldSlot == -1)
    {
        OldSlot = player->getsupplies()->selectedHotbarSlot;
    }

    player->SelectSlot(bestSlot);
}
