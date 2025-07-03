#include "AutoAnvil.h"
#include "../../../../../Client.h"
#include "../../../../../../Utils/Minecraft/Intenvoru.h"
Vec2<float> anvilRotation;
bool swa3p1 = false;
bool s1lents3p = false;
AutoAnvil::AutoAnvil() : Module("AutoAnvil", "Places anvils on enemy heads", Category::COMBAT) {
    registerSetting(new ToggleSetting("mobs", "Place on mobs", &mobs, false));
    registerSetting(new ToggleSetting("swap", "swap", &swa3p1, true));
    registerSetting(new ToggleSetting("silent swap", "swap", &s1lents3p, false));
    registerSetting(new NumberSetting<int>("y-offset", "Vertical offset", &yoffsetJ, 3, 0, 6));
    registerSetting(new NumberSetting<int>("delay", "Tick delay between placements", &delay, 5, 0, 20));
}

void AutoAnvil::onDisable() {}
void AutoAnvil::onEnable() {}

void AutoAnvil::onNormalTick(LocalPlayer* localPlayer) {
    if (!localPlayer) return;
    if (++tickCounter < delay) return;
    tickCounter = 0;
    BlockSource* region = Game::clientInstance->getRegion();
    Level* level = localPlayer->level;
    Vec3 localPos = localPlayer->getEyePos();
    for (Actor* actor : level->getRuntimeActorList()) {
        if (!actor || actor == localPlayer) continue;
        if (!TargetUtil::isTargetValid(actor, mobs)) continue;
        if (localPlayer->getPos().dist(actor->getPos()) > 6.f) continue;
        bool foundValidPosition = false;
        float currentYOffset = yoffsetJ;
        const float yOffsetStep = 1.0f;
        const int maxAttempts = 3;
        for (int attempt = 0; attempt < maxAttempts && !foundValidPosition; attempt++) {
            Vec3 targetPos = Vec3(actor->getPos().x - 1, actor->getPos().y + currentYOffset, actor->getPos().z);
            BlockPos blockPos(static_cast<int>(targetPos.x), static_cast<int>(targetPos.y), static_cast<int>(targetPos.z));
            std::vector<std::pair<BlockPos, unsigned char>> checkPositions = {
                {BlockPos(blockPos.x, blockPos.y + 1, blockPos.z), 0},
                {BlockPos(blockPos.x - 1, blockPos.y, blockPos.z), 5},
                {BlockPos(blockPos.x + 1, blockPos.y, blockPos.z), 4},
                {BlockPos(blockPos.x, blockPos.y, blockPos.z - 1), 3},
                {BlockPos(blockPos.x, blockPos.y, blockPos.z + 1), 2}
            };
            bool canPlace = false;
            unsigned char face = 1;
            for (const auto& [checkPos, checkFace] : checkPositions) {
                if (region->getBlock(checkPos)->blockLegacy->blockId != 0) {
                    canPlace = true;
                    face = checkFace;
                    break;
                }
            }
            if (!canPlace) {
                currentYOffset -= yOffsetStep;
                continue;
            }
            if (region->getBlock(blockPos)->blockLegacy->blockId != 0) {
                currentYOffset -= yOffsetStep;
                continue;
            }
            foundValidPosition = true;
            int anvilSlot = InventoryUtils::getItem(145);
            if (anvilSlot == -1) return;
            int oldSlot = localPlayer->getplayerInventory()->selectedSlot;
            if (swa3p1) {
                InventoryUtils::SwitchTo(anvilSlot);
                Game::ourPlayer()->getgameMode()->buildBlock(blockPos, face, false);
                if (s1lents3p) {
                    InventoryUtils::SwitchTo(oldSlot);
                }
            }
        }
        if (!foundValidPosition) {
            continue;
        }
    }
}