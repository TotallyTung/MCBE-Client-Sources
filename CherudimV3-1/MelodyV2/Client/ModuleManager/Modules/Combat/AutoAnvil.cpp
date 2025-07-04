#include "AutoAnvil.h"
#include <algorithm>
#include <vector>
#include <ranges>
int Anvilheight = 3;
AutoAnvil::AutoAnvil() : Module("AutoAnvil", "Automatically places anvils above players to destroy helmets or get them stuck", Category::COMBAT) {
	addBoolCheck("Attack Mob", "If u want attack mob or not", &attackMob);
	addSlider<int>("Height", "NULL", ValueType::INT_T, &Anvilheight, 3, 10);
}
static int getObsidian() {
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
static int getAnvil() {
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	int slot = plrInv->selectedSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 145) {
				return i;
			}
		}
	}
	return slot;
}

static void predictBlock(Vec3<float> pos) {
	GameMode* gm = mc.getGameMode();
	static std::vector<Vec3<float>> blocks;

	if (blocks.empty()) {
		for (int y = -Anvilheight; y <= Anvilheight; y++) {
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
static void tryBuildObsidian(Vec3<int> tryBuildPos) {
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
		if (shouldSwitch) {
			plrInv->selectedSlot = bestSlot;
			MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
			mc.getClientInstance()->loopbackPacketSender->send(&pk);
		}

		predictBlock(tryBuildPos.toFloat());

		if (shouldSwitch) {
			plrInv->selectedSlot = oldSlot;
		}
	}
}
static void tryBuildAnvil(Vec3<int> tryBuildPos) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	GameMode* gm = localPlayer->getGameMode();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;

	Vec3<float> playerPos = *localPlayer->getPosition();
	playerPos.y -= 1.f;
	playerPos = playerPos.floor();

	Block* block = localPlayer->dimension->blockSource->getBlock(tryBuildPos);
	if (block->blockLegacy->blockId == 0) {

		int bestSlot = getAnvil();
		int oldSlot = plrInv->selectedSlot;
		bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
		if (shouldSwitch) {
			plrInv->selectedSlot = bestSlot;
			MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
			mc.getClientInstance()->loopbackPacketSender->send(&pk);
		}

		gm->tryPlaceBlock(tryBuildPos);

		if (shouldSwitch) {
			plrInv->selectedSlot = oldSlot;
		}
	}
}
void AutoAnvil::onNormalTick(Actor* actor) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	GameMode* gm = mc.getGameMode();
	BlockSource* region = localPlayer->dimension->blockSource;
	Level* level = localPlayer->getLevel();
	targetList.clear();
	if (level == nullptr) return;

	for (Actor* actor : level->getRuntimeActorList()) {
		if (TargetUtils::isTargetValid(actor, attackMob)) {
			float seenPercent = region->getSeenPercent(localPlayer->getEyePos(), *actor->getAABB());
			float dist = actor->getPosition()->dist(*localPlayer->getPosition());
			float rangeCheck = (seenPercent > 0.f) ? 7 : 7;
			if (dist < rangeCheck) targetList.push_back(actor);
		}
	}
	if (!targetList.empty()) {
		std::sort(targetList.begin(), targetList.end(), TargetUtils::sortByDist);
		Vec3<int> pos = targetList[0]->getPosition()->toInt();
		tryBuildObsidian(Vec3<int>(pos.x, pos.y + Anvilheight, pos.z - 1));
		tryBuildAnvil(Vec3<int>(pos.x, pos.y + Anvilheight, pos.z));
	}
}
