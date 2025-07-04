#include "BowSpam.h"
//#include "Module.h"

BowSpam::BowSpam() : IModule(0x0, Category::COMBAT, "Send so much arrows that even the Iron Dome can't get them all") {
	registerIntSetting("Max Charge", &this->charge, this->charge, 1, 32);
}

BowSpam::~BowSpam() {
}

const char* BowSpam::getModuleName() {
	return ("BowSpam");
}

void BowSpam::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

    auto itemInHand = player->getSelectedItem();

	if (g_Data.isLeftClickDown && (player->getSelectedItemId() == 300)) {
		i++;
		//clientMessageF("Bow use: %f", i);

		if (i >= charge) {
			g_Data.getCGameMode()->releaseUsingItem();
			i = 0;
			if (shotOnce) player->completeUsingC_Item();
		}
		shotOnce = true;
	}
	shotOnce = false;
}
