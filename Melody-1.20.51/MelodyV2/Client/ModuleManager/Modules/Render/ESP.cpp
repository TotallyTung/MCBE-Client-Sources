// Created by Tony on 2024-10-10 10:01:54

#include "ESP.h"

ESP::ESP() : Module("ESP", "ESP Enities.", Category::RENDER) {
	addEnumSetting("Mode", "Changes esp modes", { "3D" }, &renderMode);
	addColorPicker("Color", "NULL", &color);
	addColorPicker("LineColor", "NULL", &lineColor);
	addBoolCheck("Mobs", "ESP Mobs", &mobs);
}

void ESP::onRender(MinecraftUIRenderContext* ctx) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	Level* level = localPlayer->getLevel();
	if (localPlayer == nullptr) return;
	if (level == nullptr) return;
	if (!mc.canUseMoveKeys()) return;

	espList.clear();

	for (Actor* ent : level->getRuntimeActorList()) {
		bool isValid = TargetUtils::isTargetValid(ent, mobs);
		if (isValid) espList.push_back(ent);
	}

	for (Actor* ent : espList) {
		switch (renderMode) {
		case 0: {
			AABB mobAABB = *ent->getAABB();
			RenderUtils::drawBox(mobAABB, color, lineColor, .3f, true, false);
;			break;
		}
		}
	}
}
