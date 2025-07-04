#include "AntiAnvil.h"
#include "../Combat/Reach.h"
#include "../../../Client.h"
AntiAnvil::AntiAnvil() : Module("Anti-Anvil", "Automatically prevents Auto Anvil by placing between you and the anvil.", Category::PLAYER) {
}
int GETOBSIDIAN() {
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	int slot = plrInv->selectedSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 720) {
				return i;
			}
		}
	}
	return slot;
}
void AntiAnvil::onNormalTick(Actor* actor) {
	LocalPlayer* lp = mc.getLocalPlayer();
	BlockSource* region = lp->dimension->blockSource;
	GameMode* gm = mc.getGameMode();
	PlayerInventory* plrInv = lp->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	int bestSlot = GETOBSIDIAN();
	int oldSlot = plrInv->selectedSlot;
	bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
	static Reach* ReachMod = (Reach*)client->moduleMgr->getModule("Reach");
	for (float i = 0; i <= ReachMod->reachValue; i++) {
		auto pos = lp->getPosition()->add(Vec3<float>(0.f, i + 3.f, 0.f));
		
		if (region->getBlock(pos.toInt())->blockLegacy->blockId == 145 && region->getBlock(pos.floor().toInt())->blockLegacy->blockName == "air") {
			
			
			if (shouldSwitch) {
				plrInv->selectedSlot = bestSlot;
			}

			gm->tryPlaceBlock(pos.floor().toInt());

			if (shouldSwitch) {
				plrInv->selectedSlot = oldSlot;
			}





		}
	}
}
