#include "Offhand.h"

Offhand::Offhand() : IModule(0, Category::PLAYER, "Automatically puts totems into your offhand") {
	Mode.addEntry("Totem", 0);
	Mode.addEntry("Shield", 1);
	registerEnumSetting("Mode", &Mode, 0);
}

const char* Offhand::getModuleName() {
	return "Offhand";
}

const char* Offhand::getModName() {
	if (Mode.getSelectedValue() == 0)
		return "Totem";
	else
		return "Shield";
}

void Offhand::onTick(GameMode* gm) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	PlayerInventoryProxy* supplies = localPlayer->getSupplies();
	Inventory* inv = supplies->inventory;

	int idSearching = (Mode.getSelectedValue() == 0 ? 568 : 355);
	ItemStack* current = localPlayer->getOffhandSlot();

	if (current->item == NULL) {
		for (int i = 0; i < 36; i++) {
			ItemStack* stack = inv->getItemStack(i);
			if (stack->getItemId() == idSearching) {
				localPlayer->setOffhandSlot(stack);
				inv->removeItem(i, 1);
				break;
			}
		}
	}
}