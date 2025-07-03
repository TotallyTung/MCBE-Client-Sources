#include "Spider.h"

Spider::Spider() : Module("Spider", "Climb walls like a spider", Category::MOVEMENT) {
	registerSetting(new SliderSetting<float>("Speed", "NULL", &speed, 0.5f, 0.1f, 1.f));
}

void Spider::onNormalTick(LocalPlayer* localPlayer) {
	if (localPlayer->hasHorizontalCollision()) {
		localPlayer->stateVector->velocity.y = speed;
		wasHorizontalCollision = true;
	}
	else {
		if (wasHorizontalCollision) {
			localPlayer->stateVector->velocity.y = 0.f;
			wasHorizontalCollision = false;
		}
	}
}