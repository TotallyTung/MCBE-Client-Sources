#include "AutoOreBreaker.h"
#include "../../../ModuleManager.h"
#include "../Player/PacketMine.h"

AutoOreBreaker::AutoOreBreaker() : Module("AutoOreBreaker", "Break ores using PacketMine realistically", Category::WORLD) {
    registerSetting(new SliderSetting<int>("Range", "Scan radius", &range, 3, 1, 12));
    registerSetting(new BoolSetting("Diamond", "Break Diamond", &diamond, true));
    registerSetting(new BoolSetting("Iron", "Break Iron", &iron, false));
    registerSetting(new BoolSetting("Gold", "Break Gold", &gold, false));
    registerSetting(new BoolSetting("Coal", "Break Coal", &coal, false));
    registerSetting(new BoolSetting("Redstone", "Break Redstone", &redstone, false));
    registerSetting(new BoolSetting("Emerald", "Break Emerald", &emerald, false));
    registerSetting(new BoolSetting("AncientDebris", "Break Ancient Debris", &ancient, false));
}

void AutoOreBreaker::onEnable() {
    breakListEmpty = true;
    tickDelay = 0;
}

bool AutoOreBreaker::isEmpty() const {
    return breakListEmpty;
}

void AutoOreBreaker::onNormalTick(LocalPlayer* localPlayer) {
    if (!localPlayer) return;

    PacketMine* packetMine = ModuleManager::getModule<PacketMine>();
    if (!packetMine || !packetMine->isEnabled()) packetMine->setEnabled(true);

    BlockSource* region = Game.getClientInstance()->getRegion();
    if (!region) return;

    std::vector<int> ores;
    if (diamond) ores.push_back(56);
    if (iron) ores.push_back(15);
    if (gold) ores.push_back(14);
    if (coal) ores.push_back(16);
    if (redstone) ores.push_back(74);
    if (emerald) ores.push_back(129);
    if (ancient) ores.push_back(526);

    Vec3<float> playerPos = localPlayer->getPos();
    BlockPos base((int)playerPos.x, (int)playerPos.y + 1, (int)playerPos.z);

    float closestDist = FLT_MAX;
    BlockPos closestOre(0, 0, 0);

    for (int x = -range; x <= range; x++) {
        for (int y = -range; y <= range; y++) {
            for (int z = -range; z <= range; z++) {
                BlockPos pos = base.add2(x, y, z);
                Block* blk = region->getBlock(pos);
                if (!blk || !blk->blockLegacy) continue;

                int id = blk->blockLegacy->blockId;
                if (std::find(ores.begin(), ores.end(), id) == ores.end()) continue;

                float dist = playerPos.dist(pos.CastTo<float>());
                if (dist < closestDist) {
                    closestDist = dist;
                    closestOre = pos;
                }
            }
        }
    }

    if (closestDist == FLT_MAX) {
        breakListEmpty = true;
        return;
    }

    breakListEmpty = false;

    Vec2<float> rot = playerPos.CalcAngle(closestOre.toFloat().add2(0.5f, 0.5f, 0.5f));
    localPlayer->rotation->presentRot = rot;

    GameMode* gm = localPlayer->gameMode;
    if (!gm) return;

    if (gm->destroyProgress <= 0.f || packetMine->getBreakPos() != closestOre) {
        bool b = false;
        gm->startDestroyBlock(closestOre, 0, b);
        packetMine->mineBlock(closestOre, 0);
    }
}
