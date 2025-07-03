#include "Surround.h"
#include <algorithm>
#include <vector>
#include <ranges>

Surround::Surround() : Module("Surround", "Place blocks around you.", Category::COMBAT) {
    registerSetting(new BoolSetting("Center", "NULL", &center, false));
    registerSetting(new BoolSetting("DisableComplete", "NULL", &disableComplete, false));
    registerSetting(new EnumSetting("Switch", "Only works for Obsidian", { "None", "Full", "Silent" }, &switchMode, 0));
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

bool trysex(const Vec3<int>& blockPos) {
    BlockSource* region = Game::getClientInstance()->getRegion();
    Block* block = region->getBlock(blockPos);
    BlockLegacy* blockLegacy = block->blockLegacy;
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
void Surround::onNormalTick(LocalPlayer* actor) {
    LocalPlayer* localPlayer = Game::getLocalPlayer();
    if (localPlayer == nullptr) return;

    Vec3<float> playerPos = localPlayer->getPos().sub(Vec3<float>(0.f, 1.f, 0.f));
    AABB playerAABB = localPlayer->getaabbShape()->aabb;
    delaytickjk++;
    if (delaytickjk < delay) return;
    delaytickjk = 0;

    for (const Vec3<float>& check : sideBlocks) {
        Vec3<float> posToCheck = playerPos.add(check);
        Vec3<int> blockPos = Vec3<int>(
            static_cast<int>(std::floor(posToCheck.x)),
            static_cast<int>(std::floor(posToCheck.y)),
            static_cast<int>(std::floor(posToCheck.z))
        );

        AABB blockAABB{
            blockPos.toFloat(),
            Vec3<float>(
                blockPos.x + 1.f,
                blockPos.y + 1.f,
                blockPos.z + 1.f
            )
        };

        if (playerAABB.intersects(blockAABB)) {
            Vec3<int> buildPos = Vec3<int>(
                static_cast<int>(std::floor(posToCheck.x + check.x)),
                static_cast<int>(std::floor(posToCheck.y + check.y)),
                static_cast<int>(std::floor(posToCheck.z + check.z))
            );
            tryBuildBlock(buildPos);
            for (int i : {-1, 1}) {
                for (int j : {-1, 1}) {
                    Vec3<int> sidePos = Vec3<int>(
                        blockPos.x + static_cast<int>(check.z * i),
                        blockPos.y,
                        blockPos.z + static_cast<int>(check.x * j)
                    );
                    tryBuildBlock(sidePos);
                }
            }
            for (int i : {-1, 1}) {
                for (int j : {-1, 1}) {
                    Vec3<int> cornerPos = Vec3<int>(
                        blockPos.x + static_cast<int>(check.z * i),
                        blockPos.y,
                        blockPos.z + static_cast<int>(check.x * j)
                    );
                    AABB cornerAABB{
                        cornerPos.toFloat(),
                        Vec3<float>(
                            cornerPos.x + 1.f,
                            cornerPos.y + 1.f,
                            cornerPos.z + 1.f
                        )
                    };

                    if (playerAABB.intersects(cornerAABB)) {
                        Vec3<int> adjustedPos = Vec3<int>(
                            cornerPos.x + static_cast<int>(check.z * i),
                            cornerPos.y,
                            cornerPos.z + static_cast<int>(check.x * j)
                        );
                        tryBuildBlock(adjustedPos);
                    }
                }
            }
        }
        else {
            tryBuildBlock(blockPos);
        }
    }

    if (Placeabove) {
        Vec3<int> abovePos = Vec3<int>(
            static_cast<int>(std::floor(playerPos.x)),
            static_cast<int>(std::floor(playerPos.y + 2.f)),
            static_cast<int>(std::floor(playerPos.z))
        );
        tryBuildBlock(abovePos);
    }

    if (antianchor) {
        Vec3<int> abovePos1 = Vec3<int>(
            static_cast<int>(std::floor(playerPos.x + 1.f)),
            static_cast<int>(std::floor(playerPos.y + 1.f)),
            static_cast<int>(std::floor(playerPos.z))
        );
        Vec3<int> abovePos2 = Vec3<int>(
            static_cast<int>(std::floor(playerPos.x)),
            static_cast<int>(std::floor(playerPos.y + 1.f)),
            static_cast<int>(std::floor(playerPos.z + 1.f))
        );
        tryBuildBlock(abovePos1);
        tryBuildBlock(abovePos2);
    }

    if (disableComplete) {
        this->setEnabled(false);
    }

    if (anchor) {
        localPlayer->setPos(playerPos);
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
void Surround::onSendPacket(Packet* packet) {

}