#include "ESP.h"

ESP::ESP() : Module("ESP", "Highlights player/entity", Category::RENDER) {
	registerSetting(new ColorSetting("Color", "NULL", &color, UIColor(255, 255, 255), false));
	registerSetting(new SliderSetting<int>("Alpha", "NULL", &alpha, 40, 0, 255));
	registerSetting(new SliderSetting<int>("LineAlpha", "NULL", &lineAlpha, 135, 0, 255));
	registerSetting(new BoolSetting("Mobs", "NULL", &mobs, false));
}	

void ESP::onLevelRender() {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (!localPlayer) return;

	for (auto& entity : localPlayer->level->getRuntimeActorList()) {
		if (TargetUtil::isTargetValid(entity, mobs)) {
			AABBShapeComponent* entityAABBShape = entity->getaabbShape();
			if (entityAABBShape)
				MCR::drawBox3dFilled(entityAABBShape->aabb,
					UIColor(color.r, color.g, color.b, alpha),
					UIColor(color.r, color.g, color.b, lineAlpha));
		}
	}
}
