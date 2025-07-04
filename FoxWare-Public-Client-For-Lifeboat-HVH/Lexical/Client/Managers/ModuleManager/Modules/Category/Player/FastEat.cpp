#include "FastEat.h"
int durationH;
FastEat::FastEat() : Module("FastEat", "Eat food faster.", Category::PLAYER) {
	registerSetting(new SliderSetting<int>("Delay", "delay", &durationH, 5, 0, 36));
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
				stack->item->setMaxUseDuration(durationH);
			}
		}
	}
}