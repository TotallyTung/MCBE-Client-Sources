#include "Hitbox.h"

Hitbox::Hitbox() : Module("Hitbox", "Increase hitbox size of entities", Category::COMBAT) {
	registerSetting(new SliderSetting<float>("Height", "NULL", &height, height, 1.8f, 5.f));
	registerSetting(new SliderSetting<float>("Width", "NULL", &width, width, 0.6f, 5.f));
	registerSetting(new BoolSetting("Mobs", "NULL", &includeMobs, includeMobs));
}

void Hitbox::onNormalTick(LocalPlayer* localPlayer) {
	Level* level = localPlayer->level;
	if (level == nullptr)
		return;

	for (auto& entity : level->getRuntimeActorList()) {
		if (TargetUtil::isFriendValid(entity, includeMobs)) {
			AABBShapeComponent* entityAABB = entity->aabbShape;
			entityAABB->aabb.size.y = height;
			entityAABB->aabb.size.x = width;
		}
	}
}
void Hitbox::onDisable() {
	Level* level = Game.getLocalPlayer()->level;
	if (level == nullptr)
		return;

	for (auto& entity : level->getRuntimeActorList()) {
		if (TargetUtil::isFriendValid(entity, includeMobs)) {
			AABBShapeComponent* entityAABB = entity->aabbShape;
			entityAABB->aabb.size.y = 1.8f;
			entityAABB->aabb.size.x = 0.6f;
		}
	}
}