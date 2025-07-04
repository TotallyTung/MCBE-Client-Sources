#include "Surround.h"
bool Placeabove = false;
Surround::Surround() : Module("Surround", "Place blocks around you.", Category::PLAYER) {
	addBoolCheck("Center", "NULL", &center);
	addBoolCheck("DisableComplete", "NULL", &disableComplete);
	addEnumSetting("Switch", "Only works for Obsidian", { "None", "Full", "Silent" }, &switchMode);
	addBoolCheck("Render", "NULL", &render);
	addColorPicker("Color", "NULL", &color);
	addColorPicker("LineColor", "NULL", &lineColor);
	addBoolCheck("Above place", "Place blocks above your head", &Placeabove);
}
int Surround::getObsidian() {
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	int slot = plrInv->selectedSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 49) {
				return i;
			}
		}
	}
	return slot;
}
#include <algorithm>
#include <vector>
#include <ranges>

void Surround::predictBlock(Vec3<float> pos) {
	GameMode* gm = mc.getGameMode();
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
		if (gm->tryPlaceBlock(currentBlock.toInt())) {
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
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	GameMode* gm = localPlayer->getGameMode();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;

	Vec3<float> playerPos = *localPlayer->getPosition();
	playerPos.y -= 1.f;
	playerPos = playerPos.floor();

	Block* block = localPlayer->dimension->blockSource->getBlock(tryBuildPos);
	if (block->blockLegacy->blockId == 0) {

		int bestSlot = getObsidian();
		int oldSlot = plrInv->selectedSlot;
		bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
		if (shouldSwitch && (switchMode == 1 || switchMode == 2)) {
			plrInv->selectedSlot = bestSlot;
			if (switchMode == 2) {
				MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
				mc.getClientInstance()->loopbackPacketSender->send(&pk);
			}
		}
		InteractPacket inter(InteractAction::LEFT_CLICK, mc.getLocalPlayer()->getRuntimeID(), tryBuildPos.toFloat());
		predictBlock(tryBuildPos.toFloat());
		mc.getClientInstance()->loopbackPacketSender->sendToServer(&inter);
		if (shouldSwitch && switchMode == 2) {
			plrInv->selectedSlot = oldSlot;
		}
	}
}
Vec3<float> sideBlocks[5] = {
    Vec3<float>(1, 0, 0),
    Vec3<float>(0, 0, 1),
    Vec3<float>(-1, 0, 0),
    Vec3<float>(0, 0, -1),
    Vec3<float>(0, -1, 0),
};

std::vector<Vec3<int>> renderPositions;

void Surround::onNormalTick(Actor* actor) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return;

    Vec3<float> playerPos = localPlayer->getPosition()->floor().sub(Vec3<float>(0.f, 1.f, 0.f));
    AABB playerAABB = *localPlayer->getAABB();

    for (Vec3<float> check : sideBlocks) {
        Vec3<int> posToCheck = (playerPos.add(check)).toInt();
        AABB blockAABB{ posToCheck.toFloat(), posToCheck.add(1, 1, 1).toFloat() };

        if (playerAABB.intersects(blockAABB)) {
            tryBuildBlock(posToCheck.add(check.toInt()));

            for (int i : {-1, 1}) {
                for (int j : {-1, 1}) {
                    Vec3<int> sidePos = posToCheck.add(check.z * i, check.y, check.x * j);
                    tryBuildBlock(sidePos);
                    renderPositions.push_back(sidePos); // Store position for rendering
                }
            }

            for (int i : {-1, 1}) {
                for (int j : {-1, 1}) {
                    Vec3<int> cornerPos = posToCheck.add(check.z * i, check.y, check.x * j);
                    AABB cornerAABB{ cornerPos.toFloat(), cornerPos.add(1, 1, 1).toFloat() };

                    if (playerAABB.intersects(cornerAABB)) {
                        Vec3<int> adjustedPos = cornerPos.add(check.z * i, 0, check.x * j);
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

    if (disableComplete) {
        this->setEnabled(false);
    }
}

void Surround::onRender(MinecraftUIRenderContext* ctx) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return;

    if (!render) return;

    Vec3<float> playerPos = *localPlayer->getPosition();
    playerPos.y -= 1.f;
    playerPos = playerPos.floor();

    AABB playerAABB = *mc.getLocalPlayer()->getAABB();

    for (Vec3<float> check : sideBlocks) {
        Vec3<int> posToCheck = (playerPos.add(check)).toInt();
        AABB blockAABB{ posToCheck.toFloat(), posToCheck.add(1, 1, 1).toFloat() };
        if (playerAABB.intersects(blockAABB)) {
            Vec3<int> extendedPos = posToCheck.add(check.toInt());
            RenderUtils::drawBox(extendedPos.toFloat(), color, lineColor, 0.3f, true, false);

            for (int i : {-1, 1}) {
                for (int j : {-1, 1}) {
                    Vec3<int> sidePos = posToCheck.add(check.z * i, check.y, check.x * j);
                    RenderUtils::drawBox(sidePos.toFloat(), color, lineColor, 0.3f, true, false);
                }
            }

            for (int i : {-1, 1}) {
                for (int j : {-1, 1}) {
                    Vec3<int> cornerPos = posToCheck.add(check.z * i, check.y, check.x * j);
                    AABB cornerAABB{ cornerPos.toFloat(), cornerPos.add(1, 1, 1).toFloat() };

                    if (playerAABB.intersects(cornerAABB)) {
                        Vec3<int> adjustedPos = cornerPos.add(check.z * i, 0, check.x * j);
                        RenderUtils::drawBox(adjustedPos.toFloat(), color, lineColor, 0.3f, true, false);
                    }
                }
            }
        }
        else {
            RenderUtils::drawBox(posToCheck.toFloat(), color, lineColor, 0.3f, true, false);
        }
    }
}

void Surround::onEnable() {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return;

    Vec3<float> playerPos = *localPlayer->getPosition();
    playerPos = playerPos.floor();
    if (center) {
        Vec3<float> yR = playerPos;
        yR.x += 0.5f;
        yR.y += 0.75f;
        yR.z += 0.5f;

        localPlayer->setPos(yR);
    }
}