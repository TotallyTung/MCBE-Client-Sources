#include "FastEat.h"

FastEat::FastEat() : Module("FastEat", "Eat food faster.", Category::PLAYER) {
}

void FastEat::onDisable() {
	if (Game.getLocalPlayer() == nullptr) return;
	Container* inv = Game.getLocalPlayer()->getplayerInventory()->container;
	for (int i = 0; i < 36; i++) {
		ItemStack* stack = inv->getItem(i);
		if (stack->item) {
			if (stack->item->isFood()) {
				stack->item->setMaxUseDuration(32);
			}
		}
	}
}

void FastEat::onNormalTick(LocalPlayer* actor) {
	Container* inv = Game.getLocalPlayer()->getplayerInventory()->container;
	for (int i = 0; i < 36; i++) {
		ItemStack* stack = inv->getItem(i);
		if (stack->item) {
			if (stack->item->isFood()) {
				stack->item->setMaxUseDuration(duration);
			}
		}
	}
}