#include "Phase.h"

Phase::Phase() : Module("Phase", "Allow you walk through blocks/walls", Category::MOVEMENT) {
}

void Phase::onDisable() {
	LocalPlayer* localPlayer = Game::getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	AABBShapeComponent* aabbShape = localPlayer->aabbShape;
	AABB& aabb = aabbShape->aabb;

	if (aabb.upper.y == aabb.lower.y)
		aabb.upper.y += 1.8f;
}

void Phase::onLevelTick(Level* level) {
	LocalPlayer* localPlayer = Game::getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	AABBShapeComponent* aabbShape = localPlayer->aabbShape;
	AABB& aabb = aabbShape->aabb;

	aabb.upper.y = aabb.lower.y;
}