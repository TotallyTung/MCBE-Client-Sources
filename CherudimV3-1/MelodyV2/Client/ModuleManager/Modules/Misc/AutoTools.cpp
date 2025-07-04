#include "AutoTools.h"

AutoTools::AutoTools() : Module("AutoTools", "Auto switch totems/shields to your offhand.", Category::MISC) {
}
int getBestDestoryItemSlot(Block* block) {

	if (block->blockLegacy->material->isSolidBlocking()) {
		auto supplies = mc.getLocalPlayer()->getPlayerInventory();
		auto inv = supplies->inventory;
		float cachedSpeed = 0.f;
		int bestSlot = 0;

		for (int i = 0; i < 9 * 4; i++) {
			auto item = inv->getItemStack(i);
			if (!item->isValid()) continue;
			float curSpeed = item->getItemPtr()->getDestroySpeed(*item,*block);
			if (curSpeed > cachedSpeed) {
				cachedSpeed = curSpeed;
				bestSlot = i;
			}
		}
		return bestSlot;
	}
	return -1;
}

void AutoTools::onNormalTick(Actor* actor) {
	LocalPlayer* localPlayer = (LocalPlayer*)actor;
	auto* gm = mc.getLocalPlayer()->getGameMode();
	// Drop useless items
	auto supplies = mc.getLocalPlayer()->getPlayerInventory();
	auto inv = supplies->inventory;
	//static auto AutoMineMod = moduleMgr->getModule<AutoMine>("AutoMine");
	auto level = localPlayer->getLevel();
	const auto& hitResult = level->mHitResult;
	if (hitResult->type == HitResultType::BLOCK && mc.isLeftClickDown()) {
		int bestSlot = getBestDestoryItemSlot(mc.getLocalPlayer()->dimension->blockSource->getBlock(hitResult->blockPos));

		if (bestSlot != mc.getLocalPlayer()->getPlayerInventory()->selectedSlot && bestSlot > -1) {
			if (bestSlot > 8) {
				int emptySlot = inv->getFirstEmptySlot();
				if (emptySlot > 8) {
					inv->swapSlots(bestSlot, supplies->selectedSlot,mc.getLocalPlayer()->getInventoryTransactionManager());
				}
				else {
					inv->swapSlots(bestSlot, emptySlot, mc.getLocalPlayer()->getInventoryTransactionManager());
				}
			}
			supplies->selectedSlot = bestSlot;
		}
	}
}
