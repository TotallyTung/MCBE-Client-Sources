#include "Hitbox.h"

HitBoxes::HitBoxes() : IModule(0, Category::COMBAT, "Increase an entitys HitBoxes size") {
	registerFloatSetting("Height", &height, height, 2.f, 4);
	registerFloatSetting("Width", &width, width, 1.f, 6);
}

const char* HitBoxes::getModuleName() {
	return "Hitbox";
}

void increaseHitbox(Entity* currentEntity, bool isRegularEntity) {
	static auto hitBoxes = moduleMgr->getModule<HitBoxes>();

	if (currentEntity == Game.getLocalPlayer()) return;
	if (!Target::isValidTarget(currentEntity)) return;

	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());
	if (dist < hitBoxes->range) {
		currentEntity->width = hitBoxes->width;
		currentEntity->height = hitBoxes->height;
	}
}

void HitBoxes::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	Game.forEachEntity(increaseHitbox);
}