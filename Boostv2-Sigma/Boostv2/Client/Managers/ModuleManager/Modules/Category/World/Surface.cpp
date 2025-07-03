#include "Surface.h"
#include "../../../../../Client.h"
#include <limits>

Surface::Surface() : Module("SurfaceTP", "Teleport to the highest surface", Category::WORLD) {}

void Surface::onEnable() {
    Game.DisplayClientMessage("%s[Surface] Enabled", MCTF::GREEN);

    lerping = false;
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    if (!localPlayer) return;

    BlockSource* region = Game.clientInstance->getRegion();
    if (!region) return;

    Vec3<float> playerPos = localPlayer->getPos();

    // Scan from Y=256 down to Y=60
    for (int y = 256; y >= 60; y--) {
        BlockPos pos(playerPos.x, y, playerPos.z);
        Block* block = region->getBlock(pos);
        if (!block || !block->blockLegacy) continue;

        int blockId = block->blockLegacy->blockId;

        // Skip air and liquids
        if (blockId == 0 || (blockId >= 8 && blockId <= 11)) continue;

        // Check that the space above is clear (air) for safe teleportation
        Block* above1 = region->getBlock(BlockPos(playerPos.x, y + 1, playerPos.z));
        Block* above2 = region->getBlock(BlockPos(playerPos.x, y + 2, playerPos.z));
        Block* above3 = region->getBlock(BlockPos(playerPos.x, y + 3, playerPos.z));

        if (!above1 || !above2 || !above3) continue;

        // If the space above is clear, set the teleport position slightly above the block
        if (above1->blockLegacy->blockId == 0 && above2->blockLegacy->blockId == 0 && above3->blockLegacy->blockId == 0) {
            // Teleport slightly above the surface block to land on top
            targetPos = Vec3<float>(playerPos.x + 0.5f, y + 1.0f, playerPos.z + 0.5f);
            lerping = true;

            Minecraft* mc = Game.clientInstance->minecraft;
            *mc->minecraftTimer = 10700.0f;
            return;
        }
    }

    Game.DisplayClientMessage("%s[Surface] No surface found", MCTF::RED);
    this->setEnabled(false);
}

void Surface::onNormalTick(LocalPlayer* localPlayer) {
    if (!localPlayer || !lerping) return;

    Vec3<float> currentPos = localPlayer->getPos();
    float distance = currentPos.dist(targetPos);

    float lerpFactor = 0.2f;
    if (distance <= 13.0f) lerpFactor = 0.55f;
    else if (distance <= 20.0f) lerpFactor = 0.45f;
    else if (distance <= 100.0f) lerpFactor = 0.10f;
    else lerpFactor = 0.075f;

    if (distance <= 0.25f) {
        localPlayer->setPos(targetPos);
        Game.DisplayClientMessage("%s[Surface] Teleported to the top", MCTF::GREEN);
        lerping = false;
        this->setEnabled(false);
        return;
    }

    Vec3<float> newPos = currentPos.lerpTo(targetPos, lerpFactor);
    localPlayer->setPos(newPos);
}

void Surface::onDisable() {
    lerping = false;
    Minecraft* mc = Game.clientInstance->minecraft;
    *mc->minecraftTimer = 20.0f;
    Game.DisplayClientMessage("%s[Surface] Disabled", MCTF::WHITE);
}
