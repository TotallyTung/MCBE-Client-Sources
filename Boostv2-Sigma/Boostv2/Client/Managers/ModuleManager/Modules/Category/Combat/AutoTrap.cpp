#include "AutoTrap.h"
#include "../../../../../Client.h"
#include "../../../../../../Utils/Minecraft/Intenvoru.h"
#include "../../../../../../Utils/ValidationUtils.h"

bool grimMode = false;

AutoTrap::AutoTrap() : Module("AutoTrap", "Fully traps enemies in obsidian", Category::COMBAT) {
    registerSetting(new BoolSetting("Mobs", "Target mobs", &targetMobs, false));
    registerSetting(new BoolSetting("Swap", "Swap to obsidian", &swap, true));
    registerSetting(new BoolSetting("Silent", "Silent swap", &silentSwap, false));
    registerSetting(new SliderSetting<int>("Delay", "Place delay (ticks)", &placeDelay, 0, 0, 20));
    registerSetting(new EnumSetting("Mode", "Trap mode", { "Full Box", "Top Only", "Feet Only", "Cage" }, &trapMode, 0));
    registerSetting(new BoolSetting("Rotate", "Face placements", &rotate, true));
    registerSetting(new BoolSetting("AirCheck", "Only place in air", &airCheck, true));
    registerSetting(new BoolSetting("Strict Mode", "Limit to one blocks per tick", &grimMode, false));
}

void AutoTrap::onDisable() {
    placedBlocks.clear();
}

void AutoTrap::onEnable() {
    placedBlocks.clear();
}

void AutoTrap::onNormalTick(LocalPlayer* localPlayer) {
    if (!ValidationUtils::isValid(localPlayer)) return;
    if (++tickCounter < placeDelay) {
        return;
    }
    tickCounter = 0;
    BlockSource* region = Game.clientInstance->getRegion();
    Level* level = localPlayer->level;
    if (!ValidationUtils::isValidContext(localPlayer, level, region)) return;
    int obsidianSlot = InventoryUtils::getItem(49);
    if (obsidianSlot == -1) return;
    for (Actor* actor : level->getRuntimeActorList()) {
        if (!actor || actor == localPlayer) continue;
        if (!TargetUtil::isTargetValid(actor, targetMobs)) continue;
        Vec3 targetPos = actor->getPos();
        BlockPos targetBlockPos(static_cast<int>(targetPos.x) - 1, static_cast<int>(targetPos.y), static_cast<int>(targetPos.z));
        std::vector<BlockPos> trapPositions;
        trapPositions.clear();
        switch (trapMode) {
        case 0:
            for (int x = -1; x <= 1; x++) {
                for (int y = 0; y <= 2; y++) {
                    for (int z = -1; z <= 1; z++) {
                        if (x == 0 && y <= 1 && z == 0) continue;
                        trapPositions.emplace_back(targetBlockPos.x + x, targetBlockPos.y + y, targetBlockPos.z + z);
                    }
                }
            }
            break;

        case 1:
            for (int x = -1; x <= 1; x++) {
                for (int z = -1; z <= 1; z++) {
                    trapPositions.emplace_back(targetBlockPos.x + x, targetBlockPos.y + 2, targetBlockPos.z + z);
                }
            }
            break;

        case 2:
            for (int x = -1; x <= 1; x++) {
                for (int z = -1; z <= 1; z++) {
                    if (x == 0 && z == 0) continue;
                    trapPositions.emplace_back(targetBlockPos.x + x, targetBlockPos.y, targetBlockPos.z + z);
                }
            }
            break;

        case 3:
            for (int y = 0; y <= 2; y++) {
                trapPositions.emplace_back(targetBlockPos.x + 1, targetBlockPos.y + y, targetBlockPos.z);
                trapPositions.emplace_back(targetBlockPos.x - 1, targetBlockPos.y + y, targetBlockPos.z);
                trapPositions.emplace_back(targetBlockPos.x, targetBlockPos.y + y, targetBlockPos.z + 1);
                trapPositions.emplace_back(targetBlockPos.x, targetBlockPos.y + y, targetBlockPos.z - 1);
            }
            break;
        }
        std::vector<std::pair<BlockPos, int>> validPositions;
        for (const auto& pos : trapPositions) {
            if (placedBlocks.count(pos)) continue;
            auto block = region->getBlock(pos);
            if (!block || !block->blockLegacy) continue;
            if (airCheck && block->blockLegacy->blockId != 0) continue;
            int bestFace = findBestPlacementFace(region, pos);
            if (bestFace == -1) continue;
            validPositions.emplace_back(pos, bestFace);
        }
        if (validPositions.empty()) continue;
        int oldSlot = localPlayer->getplayerInventory()->selectedSlot;
        if (swap) InventoryUtils::SwitchTo(obsidianSlot);
        if (grimMode) {
            int count = 0;
            for (const auto& [pos, face] : validPositions) {
                placeBlock(localPlayer, pos, face);
                if (++count >= 2) break;
            }
        }
        else {
            for (const auto& [pos, face] : validPositions) {
                placeBlock(localPlayer, pos, face);
            }
        }
        if (swap && silentSwap) InventoryUtils::SwitchTo(oldSlot);
    }
}

void AutoTrap::placeBlock(LocalPlayer* localPlayer, BlockPos pos, int face) {
    if (rotate) {
        Vec2 angle = computePlaceAngle(localPlayer->getEyePos(), Vec3(pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f));
        localPlayer->rotation->presentRot = Vec2(angle.x, angle.y);
    }
    Game.getLocalPlayer()->getgameMode()->buildBlock(pos, face, false);
    placedBlocks.insert(pos);
}

int AutoTrap::findBestPlacementFace(BlockSource* region, const BlockPos& pos) {
    static const std::vector<std::pair<BlockPos, int>> adjacentChecks = {
        {BlockPos(pos.x, pos.y - 1, pos.z), 1},
        {BlockPos(pos.x, pos.y + 1, pos.z), 0},
        {BlockPos(pos.x - 1, pos.y, pos.z), 5},
        {BlockPos(pos.x + 1, pos.y, pos.z), 4},
        {BlockPos(pos.x, pos.y, pos.z - 1), 3},
        {BlockPos(pos.x, pos.y, pos.z + 1), 2}
    };
    for (const auto& [checkPos, face] : adjacentChecks) {
        auto block = region->getBlock(checkPos);
        if (block && block->blockLegacy && block->blockLegacy->blockId != 0) {
            return face;
        }
    }
    return -1;
}

Vec2<float> AutoTrap::computePlaceAngle(const Vec3<float>& from, const Vec3<float>& to) {
    Vec3 delta = to.sub(from);
    float yaw = atan2(delta.z, delta.x) * (180 / PI) - 90;
    float pitch = -atan2(delta.y, sqrt(delta.x * delta.x + delta.z * delta.z)) * (180 / PI);
    return Vec2(yaw, pitch);
}
