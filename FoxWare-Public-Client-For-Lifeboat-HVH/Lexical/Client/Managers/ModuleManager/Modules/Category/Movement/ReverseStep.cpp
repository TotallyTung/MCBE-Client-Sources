#include "ReverseStep.h"

ReverseStep::ReverseStep() : Module("ReverseStep", "Step but reverse", Category::MOVEMENT) {
}

void ReverseStep::onNormalTick(LocalPlayer* actor) {
	Vec3<float> actorPos = actor->getPos(); actorPos.y -= 1.f;
	if (Game::getClientInstance()->getRegion()->getBlock(actorPos.toInt())->blockLegacy->blockId != 0) return;
	if (actor->hasComponent<OnGroundFlagComponent>() && !Game::isKeyDown(VK_SPACE)) actor->stateVector->velocity.y = -1;
}