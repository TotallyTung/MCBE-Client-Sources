#include "Hitbox.h"

Hitbox::Hitbox() : Module("Hitbox", "Hitbox for all niggas", Category::COMBAT) {
	addSlider<float>("Width", "NULL", ValueType::FLOAT_T, &width, 1.f, 10.f);
	addSlider<float>("Height", "NULL", ValueType::FLOAT_T, &height, 1.f, 10.f);
	addBoolCheck("Mobs", "NULL", &mobs);
}

void Hitbox::onNormalTick(Actor* actor) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();

	if (localPlayer == nullptr) return;
	if (!mc.canUseMoveKeys()) return;
	Level* level = localPlayer->getLevel();
	if (level == nullptr) return;
	espList.clear();

	for (Actor* ent : level->getRuntimeActorList()) {
		bool isValid = TargetUtils::isTargetValid(ent, mobs);
		if (isValid) espList.push_back(ent);
	}

	for (Actor* ent : espList) {
		ent->getAABB()->size.x = width;
		ent->getAABB()->size.y = height;
	}
}
