#include "AutoTool.h"

AutoTool::AutoTool() : Module("AutoTool", "Automatically switches to the most effective tool when performing an action", Category::PLAYER) {
}

int AutoTool::getBestDestoryItemSlot(Block* block) {

	if (block->blockLegacy->blockId == 0)
		return -1;

	LocalPlayer* localPlayer = Game::getLocalPlayer();
	PlayerInventory* plrInv = localPlayer->playerInventory;

	int bestSlot = plrInv->selectedSlot;
	float cachedSpeed = localPlayer->getCarriedItem()->getDestroySpeed(block);

	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = plrInv->container->getItem(i);
		float curSpeed = itemStack->getDestroySpeed(block);
		if (curSpeed > cachedSpeed) {
			bestSlot = i;
			cachedSpeed = curSpeed;
		}
	}
	return bestSlot;
}

void AutoTool::onNormalTick(LocalPlayer* localPlayer) {
	if (!Game::canUseMoveKeys())
		return;

	BlockSource* region = Game::clientInstance->getRegion();
	Level* level = localPlayer->level;
	HitResult* hitResult = level->getHitResult();

	int32_t& selectedSlot = localPlayer->playerInventory->selectedSlot;

	if (hitResult->type == HitResultType::BLOCK && Game::isLeftClickDown()) {
		int bestSlot = getBestDestoryItemSlot(region->getBlock(hitResult->blockPos));
		if (bestSlot != -1 && selectedSlot != bestSlot) {
			selectedSlot = bestSlot;
		}
	}
}