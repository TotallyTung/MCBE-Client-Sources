#include "FastEat.h"
//
FastEat::FastEat() : Module("FastEat", "Eat food faster.", Category::PLAYER) {
	registerSetting(new SliderSetting<int>("Duration", "NULL", &duration, 32, 1, 32));
}

void FastEat::onDisable() {
	if (Game::getLocalPlayer() == nullptr) return;
	Container* inv = Game::getLocalPlayer()->getplayerInventory()->container;
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
	Container* inv = Game::getLocalPlayer()->getplayerInventory()->container;
	for (int i = 0; i < 36; i++) {
		ItemStack* stack = inv->getItem(i);
		if (stack->item) {
			if (stack->item->isFood()) {
				stack->item->setMaxUseDuration(duration);
			}
		}
	}
}