#include "PacketGapple.h"
bool Healthislow;
void PacketGapple::onTick(GameMode* gm) {
	auto plr = Game.getLocalPlayer();
	if (plr == nullptr) return;
	int healthy = plr->getHealth();
	if (healthy < health) {
		int slotab = Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
		plr->switchHotbar(getGapple(plr));
		ItemStack* GappleStack = Game.getLocalPlayer()->getSupplies()->inventory->getItemStack(getGapple(plr));
		if (GappleStack != nullptr && GappleStack->getItemId() == 259) {
			if (plr->getItemUseDuration() == 0) {
				gm->baseUseItem(*GappleStack);
			}
		}
		Healthislow = true;
	}
	else if (healthy > health && Healthislow) {
		Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot = backSlot;
		Healthislow = false;
	}
};