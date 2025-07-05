#pragma once

#include "OreESP.h"

#include <mutex>
#include <Prax/SDK/Game/Render/LevelRenderer.h>
#include <Prax/SDK/Game/Internal/MinecraftGame.h>
#include <Prax/Utils/Render/Render.h>

static std::mutex blocksMutex;

void OreESP::OnEnable() {
}


static uint64_t lastUpdate = 0;
static std::vector<BlockPosIdPair> blocks = {};

constexpr int REDSTONE_ORE = 73;
constexpr int REDSTONE_ORE_LIT = 74;
constexpr int DIAMOND_ORE = 56;
constexpr int EMERALD_ORE = 129;
constexpr int GOLD_ORE = 14;
constexpr int IRON_ORE = 15;
constexpr int LAPIS_ORE = 21;
constexpr int COAL_ORE = 16;
constexpr int BARRIER = 416;
constexpr int DEEPSLATE_REDSTONE_ORE = 658;
constexpr int DEEPSLATE_LIT_REDSTONE_ORE = 659;
constexpr int DEEPSLATE_DIAMOND_ORE = 660;
constexpr int DEEPSLATE_EMERALD_ORE = 662;
constexpr int DEEPSLATE_GOLD_ORE = 657;
constexpr int DEEPSLATE_IRON_ORE = 656;
constexpr int DEEPSLATE_LAPIS_ORE = 655;
constexpr int DEEPSLATE_COAL_ORE = 661;

static std::vector validIds = {
    REDSTONE_ORE,
    REDSTONE_ORE_LIT,
    DIAMOND_ORE,
    EMERALD_ORE,
    GOLD_ORE,
    IRON_ORE,
    LAPIS_ORE,
    COAL_ORE,
    BARRIER,
    DEEPSLATE_REDSTONE_ORE,
    DEEPSLATE_LIT_REDSTONE_ORE,
    DEEPSLATE_DIAMOND_ORE,
    DEEPSLATE_EMERALD_ORE,
    DEEPSLATE_GOLD_ORE,
    DEEPSLATE_IRON_ORE,
    DEEPSLATE_LAPIS_ORE,
    DEEPSLATE_COAL_ORE
};

void OreESP::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (Misc::GetCurrentMs() - lastUpdate > UpdateFrequency.Value * 50 || UpdateFrequency.Value <= 1.f)
    {
        std::lock_guard lock(blocksMutex);
        blocks = Misc::GetBlocksWithinDistance3(player->getPos(), DistanceSetting.Value, false, false);
        lastUpdate = Misc::GetCurrentMs();
    }



}

// Called when getBlock is called
void OreESP::OnGetBlock(Vector3& blockPos, Block* block)
{

}

void OreESP::DrawBoxAroundBlock(Vector3 blockPos, int lol) {
    if (!Minecraft::ClientInstance->levelRenderer->levelRendererPlayer) return;

    AABB aabb = AABB(Vector3(blockPos.x, blockPos.y, blockPos.z), Vector3(1, 1, 1));

    ImColor color;



    // Check the block id and set the color accordingly
    if ((lol == REDSTONE_ORE || lol == REDSTONE_ORE_LIT || lol == DEEPSLATE_REDSTONE_ORE || lol == DEEPSLATE_LIT_REDSTONE_ORE) && RedstoneSetting.Enabled)
        color = ImColor(255, 0, 0, 255);
    else if ((lol == DIAMOND_ORE || lol == DEEPSLATE_DIAMOND_ORE) && DiamondSetting.Enabled)
        color = ImColor(0, 255, 255, 255);
    else if ((lol == EMERALD_ORE || lol == DEEPSLATE_EMERALD_ORE) && EmeraldSetting.Enabled)
        color = ImColor(0, 255, 0, 255);
    else if ((lol == GOLD_ORE || lol == DEEPSLATE_GOLD_ORE) && GoldSetting.Enabled)
        color = ImColor(255, 255, 0, 255);
    else if ((lol == IRON_ORE || lol == DEEPSLATE_IRON_ORE) && IronSetting.Enabled)
        color = ImColor(255, 255, 255, 255);
    else if ((lol == LAPIS_ORE || lol == DEEPSLATE_LAPIS_ORE) && LapisSetting.Enabled)
        color = ImColor(0, 0, 255, 255);
    else if ((lol == COAL_ORE || lol == DEEPSLATE_COAL_ORE) && CoalSetting.Enabled)
        color = ImColor(100, 100, 100, 255);
    else if (lol == BARRIER && BarrierSetting.Enabled)
        color = ImColor(255, 0, 0, 255);
    else return;

    Math::DrawOutlinedBox(aabb.lower, aabb.upper, 1.0f, color);
}



void OreESP::OnRender()
{
    if (!Minecraft::ClientInstance->levelRenderer->levelRendererPlayer) return;
    if (!Minecraft::ClientInstance->getmcGame()->canUseKeys()) return;

    {
        std::lock_guard lock(blocksMutex);
        for (auto& block : blocks) {
            DrawBoxAroundBlock(block.blockPos, block.blockId);
        }
    }
}

void OreESP::OnDisable() {
}