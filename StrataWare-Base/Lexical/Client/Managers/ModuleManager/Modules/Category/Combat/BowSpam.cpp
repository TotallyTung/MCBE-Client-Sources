#include "BowSpam.h"

BowSpam::BowSpam() : Module("BowSpam", "Spam arrows quickly", Category::COMBAT) {
	registerSetting(new NumberSetting<int>("Delay", "NULL", &delay, 5, 3, 32));
}
#include <Windows.h>
bool isRightClickPresse23232d() {
	return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
}
void BowSpam::onNormalTick(LocalPlayer* localPlayer) {
	if (!Game::canUseMoveKeys())
		return;

	if (!isRightClickPresse23232d())
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