#include "Hitbox.h"

Hitbox::Hitbox() : IModule(0, Category::COMBAT, "Increase an entitys hitbox size.") {
	registerFloatSetting("Height", &height, height, 0.01f, 3.f);
	registerFloatSetting("Width", &width, width, 0.01f, 3.f);
}

Hitbox::~Hitbox() {
}

const char* Hitbox::getModuleName() {
	return ("Hitbox");
}

void findTarget(Entity* currentEntity, bool isRegularEntitie) {
	static auto hitboxMod = moduleMgr->getModule<Hitbox>();
	
	if (currentEntity == Game.getLocalPlayer())  // Skip Local player
		return;

	if (currentEntity == 0)
		return;

	if (currentEntity->timeSinceDeath > 0 || currentEntity->damageTime >= 7)
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());

	if (dist < hitboxMod->range) {
		currentEntity->width = hitboxMod->width;
		currentEntity->height = hitboxMod->height;
	}
	
}

void Hitbox::onTick(GameMode* gm) {
	Game.forEachEntity(findTarget);
}
