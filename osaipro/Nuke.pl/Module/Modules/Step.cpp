#include "Step.h"
bool reverse = true;
Step::Step() : IModule(0, Category::MOVEMENT, "Increases your step height.") {
	registerFloatSetting("Height", &height, height, 1.f, 10.f);
	registerBoolSetting("reverse", &reverse, reverse);
}

Step::~Step() {
}

const char* Step::getModuleName() {
	return ("Step");
}

void Step::onTick(GameMode* gm) {
	if (!reverse) {
		gm->player->stepHeight = height;
	};
	if (reverse) {
		if (gm->player->onGround && !gm->player->isInWater() && !gm->player->isInLava())
		{
			gm->player->velocity.y = -1;
		}
	};
}
void Step::onDisable() {
	if (Game.getLocalPlayer() != nullptr) {
		if (!reverse) {
			Game.getLocalPlayer()->stepHeight = 0.5625f;
		};
	};
}
