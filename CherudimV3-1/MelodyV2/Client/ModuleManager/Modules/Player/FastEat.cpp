#include "FastEat.h"

FastEat::FastEat() : Module("FastEat", "Eat food faster.", Category::PLAYER) {
	addSlider<int>("Duration", "NULL", ValueType::INT_T, &duration, 1, 32);
}

void FastEat::onDisable() {
	if (mc.getLocalPlayer() == nullptr) return;
	Inventory* inv = mc.getLocalPlayer()->getPlayerInventory()->inventory;
	for (int i = 0; i < 36; i++) {
		ItemStack* stack = inv->getItemStack(i);
		Item* item = stack->getItemPtr();
		if (item != nullptr) {
			if (item->isFood()) {
				item->setMaxUseDuration(32);
			}
		}
	}
}

void FastEat::onNormalTick(Actor* actor) {
	Inventory* inv = mc.getLocalPlayer()->getPlayerInventory()->inventory;
	for (int i = 0; i < 36; i++) {
		ItemStack* stack = inv->getItemStack(i);
		Item* item = stack->getItemPtr();
		if (item != nullptr) {
			if (item->isFood()) {
				item->setMaxUseDuration(duration);
			}
		}
	}
}