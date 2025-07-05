#include "AutoTotem.h"

AutoTotem::AutoTotem() : IModule(0, Category::PLAYER, "Main hand AutoTotem by hank") {
	registerIntSetting("health", &this->health, this->health, 1, 20);
}

AutoTotem::~AutoTotem() {
}

const char* AutoTotem::getModuleName() {
	return ("AutoTotem");
}

void AutoTotem::onTick(C_GameMode* gm) {
	int healthy = g_Data.getLocalPlayer()->getHealth();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	int slot = supplies->selectedHotbarSlot;
	int i;
	C_InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();
	C_ItemStack* current = g_Data.getLocalPlayer()->getEquippedTotem();
	if (healthy < health) {
		if (current->item == NULL) {
			for (i = 0; i < 9; i++) {
				C_ItemStack* totem = inv->getItemStack(i);
				if (totem->item != NULL && (*totem->item)->itemId == 568) {
					slot = i;
				}
			}
			supplies->selectedHotbarSlot = slot;
		}
	}
}
