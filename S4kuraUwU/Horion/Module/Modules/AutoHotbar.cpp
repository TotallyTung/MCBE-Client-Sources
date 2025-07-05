#include "AutoHotbar.h"

AutoHotbar::AutoHotbar() : IModule(0, Category::PLAYER, "Automatically puts crystals into your hotbar") {
	registerBoolSetting("Gapple", &this->Gapple, this->Gapple);
	registerIntSetting("Slot", &this->slot2, this->slot2, 0, 8);
	registerBoolSetting("Crystal", &this->crystal, this->crystal);
	registerIntSetting("Slot", &this->slot1, this->slot1, 0, 8);

}

AutoHotbar::~AutoHotbar() {
}

const char* AutoHotbar::getModuleName() {
	return ("AutoHotbar");
}

void AutoHotbar::onTick(C_GameMode* gm) {
	if (Gapple) {
		int GapId = 0;
		GapId = 259;
		bool run = false;

		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		int item = 0;
		for (int i = 0; i < 36; i++) {
			C_ItemStack* stack = inv->getItemStack(i);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == GapId) {
					item = i;
				}
			}
		}
		C_ItemStack* stack2 = inv->getItemStack(slot2);
		auto getItem = stack2;
		auto player = g_Data.getLocalPlayer();
		if (player == nullptr) return;
		if (item != GapId && getItem->count == 0) inv->moveItem(item, slot2);
	}
	if (crystal) {
		int crystalId = 0;
		crystalId = 637;
		bool run = false;

		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		int item = 0;
		for (int i = 0; i < 36; i++) {
			C_ItemStack* stack = inv->getItemStack(i);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == crystalId) {
					item = i;
				}
			}
		}
		C_ItemStack* stack2 = inv->getItemStack(slot1);
		auto getItem = stack2;
		auto player = g_Data.getLocalPlayer();
		if (player == nullptr) return;
		if (item != crystalId && getItem->count == 0) inv->moveItem(item, slot1);
	}
}