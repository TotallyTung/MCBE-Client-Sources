#include "FastEat.h"


FastEat::FastEat() : IModule(0, Category::PLAYER, "Eat food almost instant") {
	registerIntSetting("Duration", &duration, duration, 1, 31);
}

FastEat::~FastEat() {
}

const char* FastEat::getModuleName() {
	return ("FastEat");
}

const char* FastEat::getRawModuleName() {
	return ("FastEat");
}

void FastEat::onTick(C_GameMode* gm) {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inv->getItemStack(i);
		if (stack->item != NULL && stack->getItem()->isFood()) {
			(*stack->item)->setMaxUseDuration(duration);
		}
	}
}

void FastEat::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inv->getItemStack(i);
		if (stack->item != NULL && stack->getItem()->isFood()) {
			(*stack->item)->setMaxUseDuration(32);
		}
	}
}