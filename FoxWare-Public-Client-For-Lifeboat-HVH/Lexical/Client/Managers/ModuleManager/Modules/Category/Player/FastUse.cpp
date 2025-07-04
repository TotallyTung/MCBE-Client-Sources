#include "FastUse.h"

FastUse::FastUse() : Module("FastUse", "Allows you to use items at very high speeds", Category::PLAYER) {
	registerSetting(new SliderSetting<int>("Delay", "NULL", &delay, 1, 0, 20));
	registerSetting(new SliderSetting<int>("UsePerTick", "How many time to use each tick", &usePerTick, 1, 1, 64));
	registerSetting(new BoolSetting("OnlyXP", "Only fast use for xp bottle", &onlyXP, false));
}

void FastUse::onNormalTick(LocalPlayer* localPlayer) {
	if (!Game::canUseMoveKeys())
		return;

	if (!Game::isRightClickDown())
		return;

	ItemStack* carriedItem = localPlayer->getCarriedItem();
	if (carriedItem->isValid() && carriedItem->item.get()->isThrowable()) {
		if (onlyXP && carriedItem->item.get()->itemId != 524)
			return;

		if (tickDelay >= delay) {
			for (int i = 0; i < usePerTick; i++) {
				localPlayer->gameMode->baseUseItem(carriedItem);
			}
			tickDelay = 0;
		}
		else {
			tickDelay++;
		}
	}
}