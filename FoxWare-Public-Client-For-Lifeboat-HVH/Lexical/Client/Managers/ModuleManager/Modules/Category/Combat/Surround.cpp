#include "Surround.h"
bool Placeabove = false;
float delaytickjk = 0;
float delay = 0;
bool antianchor = false;
bool anchor = false;
Surround::Surround() : Module("Surround", "Place blocks around you.", Category::PLAYER) {
    registerSetting(new BoolSetting("Center", "NULL", &center, false));
    registerSetting(new BoolSetting("DisableComplete", "NULL", &disableComplete, false));
    registerSetting(new EnumSetting("Switch", "Only works for Obsidian", { "None", "Full", "Silent" }, &switchMode, 0));
    registerSetting(new BoolSetting("Render", "NULL", &render, true));
    registerSetting(new ColorSetting("Color", "NULL", &color, { 255, 0, 0 }));
    registerSetting(new ColorSetting("LineColor", "NULL", &lineColor, { 0, 255, 0 }));
    registerSetting(new BoolSetting("Above place", "Place blocks above your head", &Placeabove, true));
    registerSetting(new BoolSetting("Anti-Anchor", "Prevent anchor damaging", &antianchor, true));
    registerSetting(new BoolSetting("Anti-Ghost", "Use with center", &anchor, false));
    registerSetting(new SliderSetting<float>("Delay", "Delay", &delay, 5.f, 0.f, 20.f));
}
int Surround::getObsidian() {
    PlayerInventory* plrInv = Game::getLocalPlayer()->getplayerInventory();
    Container* inv = plrInv->container;
    int slot = plrInv->selectedSlot;
    for (int i = 0; i < 9; i++) {
        ItemStack* itemStack = inv->getItem(i);
        if (itemStack->isValid()) {
            if (itemStack->item->itemId == 49) {
                return i;
            }
        }
    }
    return slot;
}
#include <algorithm>
#include <vector>
#include <ranges>
bool trysex(const Vec3<int>& blockPos) {
    BlockSource* region = Game::getClientInstance()->getRegion();
    Block* block = region->getBlock(blockPos);
    BlockLegacy* blockLegacy = block->blockLegacy;

    // isSolidBlocking yerine canBeBuiltOver kullanýmý
    if (blockLegacy->canBeBuiltOver(region, blockPos)) {
        Vec3<int> blok(blockPos);

        static std::vector<Vec3<int>> checklist = {
            Vec3<int>(0, -1, 0),
            Vec3<int>(0, 1, 0),
            Vec3<int>(0, 0, -1),
            Vec3<int>(0, 0, 1),
            Vec3<int>(-1, 0, 0),
            Vec3<int>(1, 0, 0)
        };

        int i = 0;

        for (const Vec3<int>& current : checklist) {
            Vec3<int> calc = blok.sub(current);
            // Yeni kontrol: canBeBuiltOver
            if (!region->getBlock(calc)->blockLegacy->canBeBuiltOver(region, calc)) {
                Game::getLocalPlayer()->getgameMode()->buildBlock(calc, i, false);
                return true;
            }
            i++;
        }
    }
    return false;
}
void Surround::predictBlock(Vec3<float> pos) {
    GameMode* gm = Game::getLocalPlayer()->getgameMode();
    static std::vector<Vec3<float>> blocks;

    if (blocks.empty()) {
        for (int y = -5; y <= 3; y++) {
            for (int x = -5; x <= 5; x++) {
                for (int z = -5; z <= 5; z++) {
                    blocks.emplace_back(Vec3<float>(x, y, z));
                }
            }
        }

        std::ranges::sort(blocks, {}, &Math::calculateDistance);
    }

    auto tryPlaceBlock = [&](const Vec3<float>& offset) {
        Vec3<float> currentBlock = (Vec3<float>(pos.floor())).add(offset);
        if (trysex(currentBlock.toInt())) {
            return true;
        }
        return false;
        };

    for (const Vec3<float>& offset : blocks) {
        if (tryPlaceBlock(offset)) {
            return;
        }
    }
}


void Surround::tryBuildBlock(Vec3<int> tryBuildPos) {
    LocalPlayer* localPlayer = Game::getLocalPlayer();
    GameMode* gm = localPlayer->getgameMode();
    PlayerInventory* plrInv = localPlayer->getplayerInventory();
    Container* inv = plrInv->container;

    Vec3<float> playerPos = localPlayer->getPos();
    playerPos.y -= 1.f;
    playerPos = playerPos.floor();

    Block* block = Game::getClientInstance()->getRegion()->getBlock(tryBuildPos);
    if (block->blockLegacy->blockId == 0) {

        int bestSlot = getObsidian();
        int oldSlot = plrInv->selectedSlot;
        bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
        if (shouldSwitch && (switchMode == 1 || switchMode == 2)) {
            plrInv->selectedSlot = bestSlot;
        }
        predictBlock(tryBuildPos.toFloat());
        if (shouldSwitch && switchMode == 2) {
            plrInv->selectedSlot = oldSlot;
        }
    }
}
Vec3<float> sideBlocks[9] = {
    Vec3<float>(1, 0, 0),
    Vec3<float>(0, 0, 1),
    Vec3<float>(-1, 0, 0),
    Vec3<float>(0, 0, -1),
    Vec3<float>(1, -1, 0),
    Vec3<float>(0, -1, 1),
    Vec3<float>(-1, -1, 0),
    Vec3<float>(0, -1, -1),
    Vec3<float>(0, -1, 0),
};

std::vector<Vec3<int>> renderPositions;

void Surround::onNormalTick(LocalPlayer* actor) {
    LocalPlayer* localPlayer = Game::getLocalPlayer();
    if (localPlayer == nullptr) return;

    Vec3<float> playerPos = localPlayer->getPos().sub(Vec3<float>(0.f, 1.f, 0.f));
    AABB playerAABB = localPlayer->getaabbShape()->aabb;

    // Increment the delay ticker
    delaytickjk++;

    // Only proceed if the delay condition is met
    if (delaytickjk < delay) return;

    // Reset the delay ticker
    delaytickjk = 0;

    for (Vec3<float> check : sideBlocks) {
        Vec3<int> posToCheck = (playerPos.add(check)).toInt();
        AABB blockAABB{ posToCheck.toFloat(), posToCheck.add((1, 1, 1)).toFloat() };

        if (playerAABB.intersects(blockAABB)) {
            tryBuildBlock(posToCheck.add(check.toInt()));

            for (int i : {-1, 1}) {
                for (int j : {-1, 1}) {
                    Vec3<int> sidePos = posToCheck.add((check.z * i, check.y, check.x * j));
                    tryBuildBlock(sidePos);
                    renderPositions.push_back(sidePos); // Store position for rendering
                }
            }

            for (int i : {-1, 1}) {
                for (int j : {-1, 1}) {
                    Vec3<int> cornerPos = posToCheck.add((check.z * i, check.y, check.x * j));
                    AABB cornerAABB{ cornerPos.toFloat(), cornerPos.add((1, 1, 1)).toFloat() };

                    if (playerAABB.intersects(cornerAABB)) {
                        Vec3<int> adjustedPos = cornerPos.add((check.z * i, 0, check.x * j));
                        tryBuildBlock(adjustedPos);
                        renderPositions.push_back(adjustedPos); // Store position for rendering
                    }
                }
            }
        }
        else {
            tryBuildBlock(posToCheck);
            renderPositions.push_back(posToCheck); // Store position for rendering
        }
    }

    if (Placeabove) {
        Vec3<int> abovePos = (playerPos.add(Vec3<float>(0.f, 2.f, 0.f))).toInt();
        tryBuildBlock(abovePos);
        renderPositions.push_back(abovePos); // Store position for rendering
    }
    if (antianchor) {
        Vec3<int> abovePo3s = (playerPos.add(Vec3<float>(1.f, 1.f, 0.f))).toInt();
        Vec3<int> abovePos2 = (playerPos.add(Vec3<float>(0.f, 1.f, 1.f))).toInt();
        tryBuildBlock(abovePo3s);
        tryBuildBlock(abovePos2);
        renderPositions.push_back(abovePo3s); // Store position for rendering
        renderPositions.push_back(abovePos2); // Store position for rendering
    }
    if (disableComplete) {
        this->setEnabled(false);
    }
    if (anchor) {
        Vec3<float> yRz = playerPos;
        localPlayer->setPos(yRz);
    }
}


void Surround::onEnable() {
    LocalPlayer* localPlayer = Game::getLocalPlayer();
    if (localPlayer == nullptr) return;

    Vec3<float> playerPos = localPlayer->getPos();
    playerPos = playerPos.floor();
    if (center) {
        Vec3<float> yR = playerPos;
        yR.x += 0.5f;
        yR.y += 0.75f;
        yR.z += 0.5f;

        localPlayer->setPos(yR);
    }
}
