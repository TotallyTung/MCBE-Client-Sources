#include "Jesus.h"

Jesus::Jesus() : Module("Jesus", "Walk over water, like Jesus.", Category::MOVEMENT) {
}

Jesus::~Jesus() {
}
void Jesus::onNormalTick(LocalPlayer* gm) {
	if (gm->getMoveInputHandler()->isSneakDown) return;

	if (Game.getClientInstance()->getRegion()->getBlock(gm->getPos().toInt())->blockLegacy->blockName.find("water") != std::string::npos) {
		gm->stateVector->velocity.y = 0.06f;
		gm->setIsOnGround(true);
		wasInWater = true;
	}
	else if (gm->isInWater() || gm->isInLava()) {
		gm->stateVector->velocity.y = 0.1f;
		gm->setIsOnGround(true);
		wasInWater = true;
	}
	else {
		if (wasInWater) {
			wasInWater = false;
			gm->stateVector->velocity.x *= 1.2f;
			gm->stateVector->velocity.x *= 1.2f;
		}
	}
}
