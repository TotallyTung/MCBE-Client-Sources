#include "AutoDupe.h"
void AutoDupe::onTick(GameMode* gm) {
	if (Game.getLocalPlayer() == nullptr) return;
	nowtickdelay += 0.1f;
	for (int i = 0; i < packet; i++) {
		if (nowtickdelay > delay) {
			Utils::sendCommand("dupe");
			nowtickdelay = 0.f;
		}
	} 
};