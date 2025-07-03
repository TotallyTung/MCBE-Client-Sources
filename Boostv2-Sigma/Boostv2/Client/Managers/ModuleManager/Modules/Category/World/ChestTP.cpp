#define NOMINMAX
#include "ChestTP.h"
#include <limits>
#include <vector>
#include <algorithm>
#include <cmath>

ChestTP::ChestTP() : Module("ChestTP", "Teleport to nearest chest between Y=60-70", Category::WORLD) {
    registerSetting(new SliderSetting<int>("Range", "Search range for chests", &range, 30, 1, 300));
}

void ChestTP::onEnable() {
    tpdone = false;
    works = false;

    LocalPlayer* localPlayer = Game.getLocalPlayer();
    if (!localPlayer) return;

    BlockSource* region = Game.clientInstance->getRegion();
    if (!region) return;

    Minecraft* mc = Game.clientInstance->minecraft;
    *mc->minecraftTimer = 10700.0f; // Speed up game tick

    Vec3<float> playerPos = localPlayer->getPos();
    closestDistance = std::numeric_limits<float>::max();
    goal = Vec3<float>(0, 0, 0);

    int minX = std::max((int)playerPos.x - range, 0);
    int maxX = (int)playerPos.x + range;
    int minZ = std::max((int)playerPos.z - range, 0);
    int maxZ = (int)playerPos.z + range;

    for (int x = minX; x <= maxX; x++) {
        for (int y = 60; y <= 70; y++) {
            for (int z = minZ; z <= maxZ; z++) {
                BlockPos pos(x, y, z);
                Block* block = region->getBlock(pos);

                if (!block || !block->blockLegacy || block->blockLegacy->blockId != 54) continue;

                // Skip if block above chest is not air
                Block* above = region->getBlock(BlockPos(x, y + 1, z));
                if (above && above->blockLegacy && above->blockLegacy->blockId != 0) continue;

                float dist = playerPos.dist(Vec3<float>(x, y, z));
                if (dist < closestDistance) {
                    closestDistance = dist;
                    goal = Vec3<float>(x + 0.5f, y + 1.0f, z + 0.5f); // Slight offset for smooth placement
                }
            }
        }
    }

    if (closestDistance < std::numeric_limits<float>::max()) {
        works = true;
        Game.DisplayClientMessage("%s[ChestTP] Chest found", MCTF::WHITE);
    }
    else {
        Game.DisplayClientMessage("%s[ChestTP] No chests found in range", MCTF::RED);
        this->setEnabled(false);
        *mc->minecraftTimer = 20.0f;
    }
}

void ChestTP::onNormalTick(LocalPlayer* localPlayer) {
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
        this->setEnabled(false); // Will trigger onDisable
    }
    else {
        Vec3<float> newPos = currentPos.lerpTo(goal, lerpFactor);
        localPlayer->setPos(newPos);
    }
}

void ChestTP::onDisable() {
    tpdone = true;
    Minecraft* mc = Game.clientInstance->minecraft;
    *mc->minecraftTimer = 20.0f; // Reset tick speed
    Game.DisplayClientMessage("%s[ChestTP] Disabled", MCTF::WHITE);
}
