#define NOMINMAX
#include "WaterTP.h"
#include <limits>
#include <vector>
#include <algorithm>
#include <cmath>

WaterTP::WaterTP() : Module("WaterTP", "Teleport to nearest water block", Category::WORLD) {
    registerSetting(new SliderSetting<int>("Range", "Search range for water blocks", &range, 30, 1, 300));
}

void WaterTP::onEnable() {
    tpdone = false;
    works = false;

    LocalPlayer* localPlayer = Game.getLocalPlayer();
    if (!localPlayer) return;

    BlockSource* region = Game.clientInstance->getRegion();
    if (!region) return;

    Minecraft* mc = Game.clientInstance->minecraft;
    *mc->minecraftTimer = 10700.0f;

    Vec3<float> playerPos = localPlayer->getPos();
    closestDistance = std::numeric_limits<float>::max();
    goal = Vec3<float>(0, 0, 0);

    int minX = std::max((int)playerPos.x - range, 0);
    int maxX = (int)playerPos.x + range;
    int minZ = std::max((int)playerPos.z - range, 0);
    int maxZ = (int)playerPos.z + range;

    for (int x = minX; x <= maxX; x++) {
        for (int y = 60; y <= 70; y++) {  // We search for water blocks in the Y range from 60 to 70
            for (int z = minZ; z <= maxZ; z++) {
                BlockPos pos(x, y, z);
                Block* block = region->getBlock(pos);

                // Check if the block is water (ID 8 or 9)
                if (!block || !block->blockLegacy || (block->blockLegacy->blockId != 8 && block->blockLegacy->blockId != 9)) continue;

                Block* above = region->getBlock(BlockPos(x, y + 1, z));
                if (above && above->blockLegacy && above->blockLegacy->blockId != 0) continue;  // Ensure there's air above

                // Calculate the distance to the player
                float dist = playerPos.dist(Vec3<float>(x, y, z));
                if (dist < closestDistance) {
                    closestDistance = dist;
                    goal = Vec3<float>(x + 0.5f, y + 1.0f, z + 0.5f);  // Teleport one block above the water block
                }
            }
        }
    }

    if (closestDistance < std::numeric_limits<float>::max()) {
        works = true;
        Game.DisplayClientMessage("%s[WaterTP] Water found", MCTF::WHITE);
    }
    else {
        Game.DisplayClientMessage("%s[WaterTP] No water found in range", MCTF::RED);
        this->setEnabled(false);
        Minecraft* mc = Game.clientInstance->minecraft;
        *mc->minecraftTimer = 20.0f;
    }
}

void WaterTP::onNormalTick(LocalPlayer* localPlayer) {
    if (!localPlayer || !works || tpdone) return;

    Vec3<float> currentPos = localPlayer->getPos();
    float distance = currentPos.dist(goal);

    float lerpFactor = 0.2f;
    if (distance <= 13.0f) lerpFactor = 0.55f;
    else if (distance <= 20.0f) lerpFactor = 0.45f;
    else if (distance <= 100.0f) lerpFactor = 0.10f;
    else lerpFactor = 0.075f;

    if (distance <= 0.25f) {
        localPlayer->setPos(goal);
        tpdone = true;
        this->setEnabled(false);
    }
    else {
        Vec3<float> newPos = currentPos.lerpTo(goal, lerpFactor);
        localPlayer->setPos(newPos);
    }
}

void WaterTP::onDisable() {
    tpdone = true;
    Minecraft* mc = Game.clientInstance->minecraft;
    *mc->minecraftTimer = 20.0f;
    Game.DisplayClientMessage("%s[WaterTP] Disabled", MCTF::WHITE);
}
