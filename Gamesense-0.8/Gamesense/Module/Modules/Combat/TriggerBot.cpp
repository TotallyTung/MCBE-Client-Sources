#include "TriggerBot.h"

TriggerBot::TriggerBot() : IModule(0, Category::UNUSED, "Attacks the entity you're looking at") {
	//registerIntSetting("APS", &APS, APS, 1, 20);
	registerIntSetting("Delay", &delay, delay, 0, 20);
}

const char* TriggerBot::getRawModuleName() {
	return ("TriggerBot");
}

void TriggerBot::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	//auto target = g_Data.getLocalPlayer()->pointingStruct->getEntity();
	auto pointing = g_Data.getLocalPlayer()->pointingStruct;

	wtfdelay++;
	if (g_Data.canUseMoveKeys()) {
		if (pointing->hasEntity()) {
			auto target = g_Data.getLocalPlayer()->pointingStruct->getEntity();
			if (target != 0 && wtfdelay >= delay) {
				if (TargetUtil::isValidTarget(target)) {
					gm->attack(target);
						player->swing();
				}
				wtfdelay = 0;
			}
		}
	}
}