#include "BowSpam.h"

BowSpam::BowSpam() : Module("BowSpam", "Spam arrows quickly", Category::COMBAT) {
	registerSetting(new SliderSetting<int>("Delay", "NULL", &delay, 5, 3, 32));
}

void BowSpam::onNormalTick(LocalPlayer* localPlayer) {
	if (!Game::canUseMoveKeys())
		return;

	if (!Game::isRightClickDown())
		return;

	static int Odelay = 0;
	ItemStack* carriedItem = localPlayer->getCarriedItem();
	if (carriedItem->isValid() && carriedItem->item.get()->itemId == 306) {
		if (localPlayer->getItemUseDuration() == 0) {
			localPlayer->gameMode->baseUseItem(carriedItem);
			Odelay = 0;
		}
		else {
			if (Odelay >= delay) {
				localPlayer->gameMode->releaseUsingItem();
				Odelay = 0;
			}
			else {
				Odelay++;
			}
		}
	}
	else {
		Odelay = 0;
	}
}