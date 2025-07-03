// Created by Tony on 2024-10-10 10:01:54

#include "AutoEat.h"

AutoEat::AutoEat() : Module("AutoEat", "Automatic eat when u holding food.", Category::PLAYER) {
}

void AutoEat::onNormalTick(Actor* actor) {
	LocalPlayer* localPlayer = (LocalPlayer*)actor;
	GameMode* gm = mc.getGameMode();
	ItemStack* itemStack = localPlayer->getCarriedItem();
	Item* item = itemStack->getItemPtr();
	if (item != nullptr  && item->isFood()) {
		if (localPlayer->getItemUseDuration() == 0) {
			gm->baseUseItem(itemStack);
			//gm->useItem(*itemStack);

		}
	}
}