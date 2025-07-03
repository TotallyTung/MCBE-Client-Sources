#define NOMINMAX
#include "AutoMine.h"
#include <limits>
#include <vector>
#include <algorithm>
#include <cmath>

AutoMine::AutoMine() : Module("AutoMine", "Teleport to ores", Category::WORLD) {
    registerSetting(new SliderSetting<int>("Range", "Search range for ores", &range, 30, 3, 300));
    registerSetting(new BoolSetting("AntiGravel", "Avoid ores under gravel", &antiGravel, false));
    registerSetting(new BoolSetting("AntiBorder", "Avoid ores near borders", &antiBorder, false));
    registerSetting(new BoolSetting("AntiLava", "Avoid ores near lava", &antiLava, true));  // Avoid lava
    registerSetting(new BoolSetting("Diamond", "Target Diamond ore", &dim, false));
    registerSetting(new BoolSetting("Iron", "Target Iron ore", &iron, false));
    registerSetting(new BoolSetting("Coal", "Target Coal ore", &coal, false));
    registerSetting(new BoolSetting("Gold", "Target Gold ore", &gold, false));
    registerSetting(new BoolSetting("Emerald", "Target Emerald ore", &emerald, false));
    registerSetting(new BoolSetting("AncientDebris", "Target Ancient Debris", &ancient, false));
    registerSetting(new BoolSetting("Redstone", "Target Redstone ore", &redstone, false));
    registerSetting(new BoolSetting("Quartz", "Target Quartz ore", &quartz, false));
    registerSetting(new BoolSetting("Spawners", "Target Spawners", &spawners, false));
    registerSetting(new BoolSetting("Custom", "Enable custom block ID", &cuOre, false));
    registerSetting(new SliderSetting<int>("CustomID", "Custom Block ID", &cuID, 1, 0, 252));
    registerSetting(new SliderSetting<int>("Vein Size", "Min cluster size", &clusterSize, 1, 1, 9));
    registerSetting(new BoolSetting("MidClickTP", "Teleport with middle mouse click", &midClickTP, false));
}

void AutoMine::onEnable() {
    Game.DisplayClientMessage("%s[AutoMine] Made by dani (goat)", MCTF::WHITE);
    tpdone = false;
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    if (!localPlayer) return;

    BlockSource* region = Game.clientInstance->getRegion();
    if (!region) return;

    closestDistance = std::numeric_limits<float>::max();
    goal = Vec3<float>(0, 0, 0);

    Minecraft* mc = Game.clientInstance->minecraft;
    *mc->minecraftTimer = 10700.0f;

    Vec3<float> playerPos = localPlayer->getPos();
    int minX = std::max((int)playerPos.x - range, 0);
    int maxX = (int)playerPos.x + range;
    int minZ = std::max((int)playerPos.z - range, 0);
    int maxZ = (int)playerPos.z + range;

    std::vector<int> targetOres;
    if (coal) targetOres.push_back(16);
    if (iron) targetOres.push_back(15);
    if (gold) targetOres.push_back(14);
    if (redstone) targetOres.push_back(74);
    if (dim) targetOres.push_back(56);
    if (emerald) targetOres.push_back(129);
    if (ancient) targetOres.push_back(526);
    if (quartz) targetOres.push_back(153);
    if (spawners) targetOres.push_back(52);
    if (cuOre) targetOres.push_back(cuID);

    for (int x = minX; x <= maxX; x++) {
        for (int y = 5; y <= 128; y++) {
            for (int z = minZ; z <= maxZ; z++) {
                BlockPos blockPos(x, y, z);
                Block* block = region->getBlock(blockPos);
                if (!block || !block->blockLegacy) continue;

                int blockId = block->blockLegacy->blockId;
                if (std::find(targetOres.begin(), targetOres.end(), blockId) == targetOres.end()) continue;

                // Check cluster around block for total count and full anti checks
                int clusterCount = 0;
                bool clusterInvalid = false;

                for (int dx = -1; dx <= 1 && !clusterInvalid; dx++) {
                    for (int dy = -1; dy <= 1 && !clusterInvalid; dy++) {
                        for (int dz = -1; dz <= 1 && !clusterInvalid; dz++) {
                            BlockPos neighborPos(x + dx, y + dy, z + dz);
                            Block* neighborBlock = region->getBlock(neighborPos);
                            if (!neighborBlock || !neighborBlock->blockLegacy) continue;
                            if (neighborBlock->blockLegacy->blockId != blockId) continue;

                            clusterCount++;

                            // AntiGravel
                            if (antiGravel) {
                                Block* aboveBlock = region->getBlock(BlockPos(neighborPos.x, neighborPos.y + 1, neighborPos.z));
                                if (aboveBlock && aboveBlock->blockLegacy && aboveBlock->blockLegacy->blockId == 13) {
                                    clusterInvalid = true;
                                    break;
                                }
                            }

                            // AntiBorder
                            if (antiBorder) {
                                if (neighborPos.x > 1090 || neighborPos.x < 50 || neighborPos.z > 1090 || neighborPos.z < 50) {
                                    clusterInvalid = true;
                                    break;
                                }
                            }

                            // AntiLava
                            if (antiLava) {
                                for (int lx = -10; lx <= 10 && !clusterInvalid; lx++) {
                                    for (int ly = -10; ly <= 10 && !clusterInvalid; ly++) {
                                        for (int lz = -10; lz <= 10 && !clusterInvalid; lz++) {
                                            BlockPos lavaCheckPos = neighborPos.add2(lx, ly, lz);
                                            Block* lavaBlock = region->getBlock(lavaCheckPos);
                                            if (lavaBlock && lavaBlock->blockLegacy) {
                                                int lavaId = lavaBlock->blockLegacy->blockId;
                                                if (lavaId == 10 || lavaId == 11) {
                                                    clusterInvalid = true;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if (clusterInvalid) continue;
                if (clusterCount < clusterSize) continue;

                float dist = playerPos.dist(Vec3<float>(x, y, z));
                if (dist < closestDistance) {
                    closestDistance = dist;
                    goal = Vec3<float>(x + 0.5f, y + 1.0f, z + 0.5f);
                }
            }
        }
    }

    if (closestDistance < std::numeric_limits<float>::max()) {
        works = true;
        Game.getClientInstance()->playUI("random.levelup", 1, 0.6f);
        Game.DisplayClientMessage("%s[AutoMine] Found block", MCTF::WHITE);
    }
    else {
        Game.DisplayClientMessage("%s[AutoMine] No ores found", MCTF::RED);
        this->setEnabled(false);
        *mc->minecraftTimer = 20.0f;
    }
}


void AutoMine::onNormalTick(LocalPlayer* actor) {
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    if (!localPlayer || !works) return;

    // Detect mid-click input (mouse button 2) without triggering module disable
    bool midClickPressed = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0; // Mid-click detection

    if (midClickPressed && midClickTP) {
        Vec3<float> currentPos = localPlayer->getPos();
        float distance = currentPos.dist(goal);
        float lerpFactor = 0.25f;

        if (distance <= 0.25f) {
            localPlayer->setPos(goal);
        }
        else {
            Vec3<float> newPos = currentPos.lerpTo(goal, lerpFactor);
            localPlayer->setPos(newPos);
        }
        return;
    }

    if (!midClickPressed) {
        if (!tpdone) {
            Vec3<float> currentPos = localPlayer->getPos();
            float distance = currentPos.dist(goal);

            if (distance <= 1.0f) {
                localPlayer->setPos(goal);
                this->setEnabled(false);
                return;
            }

            float lerpFactor = 0.2f;
            if (distance <= 13.0f) lerpFactor = 0.55f;
            else if (distance <= 20.0f) lerpFactor = 0.45f;
            else if (distance <= 100.0f) lerpFactor = 0.10f;
            else lerpFactor = 0.075f;

            Vec3<float> newPos = currentPos.lerpTo(goal, lerpFactor);
            localPlayer->setPos(newPos);
        }
    }
}

void AutoMine::onDisable() {
    tpdone = true;
    Minecraft* mc = Game.clientInstance->minecraft;
    *mc->minecraftTimer = 20.0f;
    Game.DisplayClientMessage("%s[AutoMine] Disabled", MCTF::WHITE);
}