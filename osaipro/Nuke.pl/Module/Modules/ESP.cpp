#include "ESP.h"

ESP::ESP() : IModule(0, Category::VISUAL, "Outlines players") {
	registerBoolSetting("Mob ESP", &mobs, mobs);
	registerBoolSetting("Item ESP", &itemESP, itemESP);
}

const char* ESP::getModuleName() {
	return "ESP";
}
bool isvallllid(Entity* ent) {

	auto localPlayer = Game.getLocalPlayer();

	if (ent == localPlayer)
		return false;

	if (!Game.isInGame())
		return false;
	if (!ent->isAlive())
		return false;
	auto entityTypeId = ent->getEntityTypeId();
	if (entityTypeId != 319) return false;
	if (!localPlayer->canAttack(ent, false))
		return false;

	return (ent != nullptr);
	return true;
}

void doRenderStuff(Entity* ent, bool isRegularEntitie) {
	static auto esp = moduleMgr->getModule<ESP>();

	auto player = Game.getLocalPlayer();
	// if (ent->timeSinceDeath > 0) return;

	if (isvallllid(ent)) {
		if (!FriendsManager::findFriend(ent->getNameTag()->getText())) {
			DrawUtils::setColor(0.9f, 0.9f, 0.9f, (float)fmax(0.1f, (float)fmin(1.f, 15 / (ent->damageTime + 1))));
		}
		if (FriendsManager::findFriend(ent->getNameTag()->getText())) {
			DrawUtils::setColor(0.f, 1.f, 0.f, (float)fmax(0.1f, (float)fmin(1.f, 15 / (ent->damageTime + 1))));
		}
	}
	else if (esp->mobs) {
		if (ent->getNameTag()->getTextLength() <= 1 && ent->getEntityTypeId() == 63)
			return;

		if (ent->isInvisible())
			return;

		if (!Game.getLocalPlayer()->canAttack(ent, false))
			return;
		if (ent == Game.getLocalPlayer())
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
	DrawUtils::drawEntityBox(ent, (float)fmax(0.5f, 1 / (float)fmax(1, player->eyePos0.dist(ent->eyePos0))));
}

void ESP::onLevelRender() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
}
void ESP::onPreRender(MinecraftUIRenderContext* renderCtx) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	if (Game.canUseMoveKeys()) Game.forEachEntity(doRenderStuff);
}