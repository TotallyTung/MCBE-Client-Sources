#include "ESP.h"

ESP::ESP() : IModule(0, Category::VISUAL, "Outlines players") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Normal", 0);
	mode.addEntry("Hitbox", 1);
	mode.addEntry("2D", 2);
	registerBoolSetting("Tracers", &tracers, tracers);
	registerBoolSetting("Item ESP", &itemESP, itemESP);
}

const char* ESP::getModuleName() {
	return "ESP";
}

void doRenderStuff(Entity* ent, bool isRegularEntitie) {
	static auto esp = moduleMgr->getModule<ESP>();

	auto player = Game.getLocalPlayer();
	// if (ent->timeSinceDeath > 0) return;

	if (Target::isValidTarget(ent))
		DrawUtils::setColor(0.9f, 0.9f, 0.9f, (float)fmax(0.1f, (float)fmin(1.f, 15 / (ent->damageTime + 1))));
	else if (esp->mobs) {
		if (ent->getNameTag()->getTextLength() <= 1 && ent->getEntityTypeId() == 63)
			return;

		if (ent->isInvisible())
			return;

		if (!Game.getLocalPlayer()->canAttack(ent, false))
			return;
		DrawUtils::setColor(0.2f, 0.2f, 0.9f, (float)fmax(0.1f, (float)fmin(1.f, 15 / (ent->damageTime + 1))));
	} else if (esp->itemESP) {
		if (ent->getEntityTypeId() == 64) {
			DrawUtils::setColor(0.f, 0.9f, 0.f, 0.9f);
		} else {
			return;
		}
	} else
		return;

	if (esp->mode.getSelectedValue() == 2)
		DrawUtils::draw2D(ent, (float)fmax(0.3f, 1 / (float)fmax(1, (*player->getPos()).dist(*ent->getPos()) * 3.f)));
	else
		DrawUtils::drawEntityBox(ent, (float)fmax(0.2f, 1 / (float)fmax(1, (*player->getPos()).dist(*ent->getPos()))));
}

void ESP::onLevelRender() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	if (tracers) {
		/*auto cameraMgr = Game.getClientInstance()->getCameraManager();
		if(cameraMgr == nullptr) return;
		auto cam = cameraMgr->getCameraOrDebugCamera();
		if(cam == nullptr) return;
		Vec3 forward{};
		cam->getForwardVector(&forward);*/
		// idk how to find that, too lazy to update soooooooo lets do it like jetpack, LMAO og ass comment

		float calcYaw = (player->yaw + 90) * (PI / 180);
		float calcPitch = (player->pitch) * -(PI / 180);

		Vec3 moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * 0.5f;
		moveVec.y = sin(calcPitch) * 0.5f;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * 0.5f;

		const Vec3 origin = Game.getClientInstance()->levelRenderer->getOrigin().add(moveVec);
		Game.forEachEntity([&](Entity* ent, bool valid) {
			if (ent != Game.getLocalPlayer() && Target::isValidTarget(ent)) {
				DrawUtils::setColor(255, 255, 255, 1);
				DrawUtils::drawLine3d(origin, *ent->getPos());
			}
		});
	}
}

void ESP::onPreRender(MinecraftUIRenderContext* renderCtx) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	if (Game.canUseMoveKeys()) Game.forEachEntity(doRenderStuff);
}