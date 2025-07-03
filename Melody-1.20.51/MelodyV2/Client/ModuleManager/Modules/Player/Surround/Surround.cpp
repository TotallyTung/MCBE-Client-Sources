// Created by Tony on 2024-10-10 10:01:54

#include "Surround.h"
#include "../../../../../Utils/TessRenderUtils.h"
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
#include <unordered_set>
void Surround::onNormalTick(Actor* actor) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (localPlayer == nullptr) return;

	Vec3<float> playerPos = localPlayer->getPosition()->floor().sub(Vec3<float>(0.f, 1.f, 0.f));
	AABB playerAABB = *localPlayer->getAABB();

	std::unordered_set<Vec3<int>, Vec3Hash> blocksToPlace;

	auto addBlockToPlace = [&](const Vec3<int>& pos) {
		blocksToPlace.insert(pos);
		};
	 
	for (const Vec3<float>& check : sideBlocks) {
		Vec3<int> posToCheck = (playerPos.add(check)).toInt();
		AABB blockAABB{ posToCheck.toFloat(), posToCheck.add(1, 1, 1).toFloat() };

		if (playerAABB.intersects(blockAABB)) {
			addBlockToPlace(posToCheck.add(Vec3<int>(static_cast<int>(check.x), static_cast<int>(check.y), static_cast<int>(check.z))));

			for (int i : {-1, 1}) {
				for (int j : {-1, 1}) {
					Vec3<int> sidePos = posToCheck.add(static_cast<int>(check.z) * i, static_cast<int>(check.y), static_cast<int>(check.x) * j);
					addBlockToPlace(sidePos);

					Vec3<int> cornerPos = posToCheck.add(static_cast<int>(check.z) * i, static_cast<int>(check.y), static_cast<int>(check.x) * j);
					AABB cornerAABB{ cornerPos.toFloat(), cornerPos.add(1, 1, 1).toFloat() };

					if (playerAABB.intersects(cornerAABB)) {
						Vec3<int> adjustedPos = cornerPos.add(static_cast<int>(check.z) * i, 0, static_cast<int>(check.x) * j);
						addBlockToPlace(adjustedPos);
					}
				}
			}
		}
		else {
			addBlockToPlace(posToCheck);
		}
	}

	if (Placeabove) {
		Vec3<int> abovePos = (playerPos.add(Vec3<float>(0.f, 2.f, 0.f))).toInt();
		addBlockToPlace(abovePos);
	}

	for (const auto& pos : blocksToPlace) {
		tryBuildBlock(pos);
	}

	renderPositions.assign(blocksToPlace.begin(), blocksToPlace.end());

	if (disableComplete) {
		this->setEnabled(false);
	}
}
/*  auto lep = std::reinterpret_pointer_cast<LevelEventPacket>(packet);
        if (lep->eventId == 3600)*/
void Surround::onImGuiRender(ImDrawList* drawList) {
	if (!render) return;
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (localPlayer == nullptr) return;
	if (!mc.getClientInstance()->minecraftGame->canUseKeys) return;

	ImGuiUtils::setDrawList(drawList);

	Vec3<float> playerPos = localPlayer->getPosition()->floor().sub(Vec3<float>(0.f, 1.f, 0.f));

	for (const auto& pos : renderPositions) {
		AABB blockAABB;
		blockAABB.lower = pos.toFloat();
		blockAABB.upper = blockAABB.lower.add(1.f, 1.f, 1.f);

		ImGuiUtils::drawBox(blockAABB, color, lineColor, 1.0f, true, true);
	}

	// Render the block under the player's feet
	AABB feetAABB;
	feetAABB.lower = playerPos.sub(Vec3<float>(0.f, 1.f, 0.f));
	feetAABB.upper = feetAABB.lower.add(1.f, 1.f, 1.f);

	ImGuiUtils::drawBox(feetAABB, color, lineColor, 1.0f, true, true);
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