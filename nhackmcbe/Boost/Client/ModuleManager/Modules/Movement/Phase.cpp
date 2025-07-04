#include "Phase.h"

Phase::Phase() : Module("Phase", "Walk straight through walls.", Category::MOVEMENT) {
}

void Phase::onDisable() {
	if (mc.getLocalPlayer() != nullptr)
		mc.getLocalPlayer()->getAABB()->upper.y = mc.getLocalPlayer()->getAABB()->lower.y + 1.6f;
}

void Phase::onNormalTick(Actor* actor) {
	mc.getLocalPlayer()->getAABB()->upper.y = mc.getLocalPlayer()->getAABB()->lower.y;
}