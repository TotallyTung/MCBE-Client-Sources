#include "AutoAFKMine.h"
#include "../../../ModuleManager.h"
#include "../Player/PacketMine.h"
bool restartNextTick = false;

bool autoEat;

AfkMine::AfkMine() : Module("AutoGrind", "Grind afk", Category::WORLD) {
    registerSetting(new BoolSetting("AutoEat", "Eat automatically", &autoEat, false));
    registerSetting(new SliderSetting<int>("Range", "Scan radius", &range, 3, 1, 12));
    registerSetting(new BoolSetting("Diamond", "Break Diamond", &diamond, true));
    registerSetting(new BoolSetting("Iron", "Break Iron", &iron, false));
    registerSetting(new BoolSetting("Gold", "Break Gold", &gold, false));
    registerSetting(new BoolSetting("Coal", "Break Coal", &coal, false));
    registerSetting(new BoolSetting("Redstone", "Break Redstone", &redstone, false));
    registerSetting(new BoolSetting("Emerald", "Break Emerald", &emerald, false));
    registerSetting(new BoolSetting("AncientDebris", "Break Ancient Debris", &ancient, false));
}

void AfkMine::onEnable() {
    Game.DisplayClientMessage("%s[AutoGrind] Enabled", MCTF::WHITE);
    Game.DisplayClientMessage("%s[WARNING] Make sure your AutoMine and PacketMine settings are correct", MCTF::RED);
    AutoMine* autoMine = ModuleManager::getModule<AutoMine>();
    if (!autoMine || !autoMine->isEnabled()) autoMine->setEnabled(true);
}

int getFoodSlot() {
    auto* plrInv = Game.getLocalPlayer()->getplayerInventory();
    auto* inv = plrInv->container;

    for (int i = 0; i < 9; i++) {
        auto* itemStack = inv->getItem(i);
        if (itemStack->isValid() && itemStack->item.get() != nullptr && itemStack->item->isFood()) {
            return i;  // Return the slot index of food
        }
    }
    return plrInv->selectedSlot;  // Return selected slot if no food is found
}
void AfkMine::onNormalTick(LocalPlayer* localPlayer) {
    if (!localPlayer) return;

    if (restartNextTick) {
        restartNextTick = false;
        return;
    }

    GameMode* gm = localPlayer->gameMode;
    if (!gm) return;

    PacketMine* packetMine = ModuleManager::getModule<PacketMine>();
    if (!packetMine || !packetMine->isEnabled()) packetMine->setEnabled(true);

    BlockSource* region = Game.getClientInstance()->getRegion();
    if (!region) return;

    if (autoEat) {
        auto hunger = localPlayer->getAttribute(AttributeId::PlayerHunger);
        if (hunger->currentValue < hunger->maximumValue) {
            int foodSlot = getFoodSlot();
            if (foodSlot != -1 && localPlayer->getItemUseDuration() == 0) {
                auto* plrInv = localPlayer->getplayerInventory();
                int oldSlot = plrInv->selectedSlot;
                plrInv->selectedSlot = foodSlot;
                auto* itemStack = plrInv->container->getItem(foodSlot);
                gm->baseUseItem(itemStack);
                plrInv->selectedSlot = oldSlot;
            }
        }
    }

    AutoMine* autoMine = ModuleManager::getModule<AutoMine>();
    if (autoMine && autoMine->isEnabled())
        return;

    if (collectingItems) {
        if (currentItem < itemTPList.size()) {
            Vec3<float> target = itemTPList[currentItem] + Vec3<float>(0.f, 0.3f, 0.f);
            Vec3<float> currentPos = localPlayer->getPos();
            float dist = currentPos.dist(target);

            if (dist <= 0.6f) {
                localPlayer->setPos(target);
                currentItem++;
            }
            else {
                Vec3<float> newPos = currentPos.lerpTo(target, 0.55f);
                localPlayer->setPos(newPos);
            }
            return;
        }
        else {
            collectingItems = false;
            itemTPList.clear();
            currentItem = 0;
            return;
        }
    }

    std::vector<int> ores;
    if (diamond) ores.push_back(56);
    if (iron) ores.push_back(15);
    if (gold) ores.push_back(14);
    if (coal) ores.push_back(16);
    if (redstone) ores.push_back(74);
    if (emerald) ores.push_back(129);
    if (ancient) ores.push_back(526);

    Vec3<float> playerPos = localPlayer->getPos();
    BlockPos base((int)playerPos.x, (int)playerPos.y, (int)playerPos.z);

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
        Game.DisplayClientMessage("%s[AutoGrind] No ores nearby. Trying AutoMine or itemTP...", MCTF::WHITE);

        if (autoMine && !autoMine->isEnabled())
            autoMine->setEnabled(true);

        itemTPList.clear();
        currentItem = 0;

        for (auto& entity : localPlayer->level->getRuntimeActorList()) {
            if (entity && entity->getActorTypeComponent()->id == 64) {
                Vec3<float> entPos = entity->getPos();
                if (playerPos.dist(entPos) <= 12.f)
                    itemTPList.push_back(entPos);
            }
        }

        if (!itemTPList.empty()) {
            collectingItems = true;
            Game.DisplayClientMessage("%s[AutoGrind] Collecting nearby dropped items...", MCTF::GREEN);
        }
        else {
            Game.DisplayClientMessage("%s[AutoGrind] No ores or items. Restarting loop...", MCTF::GRAY);
            restartNextTick = true;
        }

        return;
    }

    Vec2<float> rot = playerPos.CalcAngle(closestOre.toFloat().add2(0.5f, 0.5f, 0.5f));
    localPlayer->rotation->presentRot = rot;

    if (gm->destroyProgress <= 0.f || packetMine->getBreakPos() != closestOre) {
        bool b = false;
        gm->startDestroyBlock(closestOre, 0, b);
        packetMine->mineBlock(closestOre, 0);
    }
}


void AfkMine::onDisable() {
    Game.DisplayClientMessage("%s[AutoGrind] Disabled", MCTF::WHITE);
    PacketMine* packetMine = ModuleManager::getModule<PacketMine>();
    if (!packetMine || !packetMine->isEnabled()) packetMine->setEnabled(false);
}
