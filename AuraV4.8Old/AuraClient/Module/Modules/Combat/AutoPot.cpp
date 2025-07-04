#include "AutoPot.h"

using namespace std;
AutoPot::AutoPot() : IModule(0, Category::COMBAT, "Shoot crossbow arrow for an instant") {
	registerIntSetting("StartSlot", &start, start, 1, 9);
	registerIntSetting("EndSlot", &slot, slot, 1, 9);
}

const char* AutoPot::getModuleName() {
	return ("AutoPot+");
}

//bug: replenish failure: keep making crossbow sound when reloading crossbow after shot

void AutoPot::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	auto selectedItem = player->getSelectedItem();

	if (g_Data.canUseMoveKeys() && !moduleMgr->getModule<ClickGUIMod>()->isEnabled()) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		auto prevSlot = supplies->selectedHotbarSlot;

		for (int n = start - 1; n < slot; n++) {
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 101) {  // select crystal
					supplies->selectedHotbarSlot = n;
					gm->useItem(*stack);

					// return;
				}
			}
		}
		supplies->selectedHotbarSlot = prevSlot;
	}
	static auto AutoConfigMod = moduleMgr->getModule<AutoPot>();
	AutoConfigMod->setEnabled(false);
	// setEnabled(false);
}
