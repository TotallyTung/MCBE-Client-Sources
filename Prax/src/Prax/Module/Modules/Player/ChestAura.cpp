//
// Created by vastrakai on 2/11/2024.
//

#include "ChestAura.h"

#include <Prax/Utils/Render/Render.h>

std::vector<Vector3> ChestAura::OpenedChests;
uint64_t ChestAura::LastOpenTime = 0;

#define NOW Misc::GetCurrentMs()

void ChestAura::OnEnable()
{

}

void ChestAura::OnDisable()
{
    OpenedChests.clear();
    LastOpenTime = 0;
}



void ChestAura::OnTick()
{
    if (NOW - LastOpenTime < Delay.Value) return;
    if (Minecraft::ClientInstance->GetTimerMultiplier() != 1.f) return;

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (player->isImmobile()) return;

    bool canUseKeys = Minecraft::ClientInstance->GetScreenName() == "hud_screen";
    if (!canUseKeys) return;

    std::vector<BlockPosPair> chests = Misc::GetBlocksWithinDistance2(player->getPos(), Range.Value, false, true);

    /*
    struct BlockPosPair {
        class Block* block = nullptr;
        Vector3 blockPos = Vector3();
    }*/
    // Exclude all blocks that arent chests
    chests.erase(std::ranges::remove_if(chests, [](BlockPosPair pair) {
        return !pair.block->blockLegacy->name.contains("chest");
    }).begin(), chests.end());

    // Exclude all blocks that have already been opened
    chests.erase(std::ranges::remove_if(chests, [](BlockPosPair pair) {
        return std::find(OpenedChests.begin(), OpenedChests.end(), pair.blockPos) != OpenedChests.end();
    }).begin(), chests.end());

    if (chests.empty()) return;

    auto chest = chests[0];
    auto chestPos = chest.blockPos;
    auto chestBlock = chest.block;

    int closestExposedFace = -1;
    float closestDistance = -1;

    for (int i = 0; i < 6; i++)
    {
        Vector3 facePos = chestPos - Misc::SideMap[i];
        float distance = facePos.Distance(*player->getPos());
        if (closestDistance == -1 || distance < closestDistance)
        {
            closestDistance = distance;
            closestExposedFace = i;
        }
    }

    // Open the chest by interacting with it (use buildBlock)
    player->gamemode->buildBlock(chestPos, closestExposedFace, false);

    // Add the chest to the list of opened chests
    OpenedChests.push_back(chestPos);
    LastOpenTime = NOW;
}

void ChestAura::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel)
{
    if (packet->getId() == (int)PacketID::ChangeDimension) {
        OpenedChests.clear();
    }
}
