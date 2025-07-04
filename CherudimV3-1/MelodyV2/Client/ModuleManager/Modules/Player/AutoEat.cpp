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