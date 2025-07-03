#include "AutoPhase.h"

#include "../../DrawUtils.h"
float cliping = 0.19;
AutoPhase::AutoPhase() : IModule(0x0, Category::MOVEMENT, "AutoPhase") {
	registerFloatSetting("Reduces", &cliping, 0.19, 0.f, 0.5f);
};
AutoPhase::~AutoPhase(){};

const char* AutoPhase::getModuleName() {
	return "Burrow";
}
void AutoPhase::onTick(GameMode* gm) {
	if (!Game.isInGame()) return;
	if (gm->player->aabb.upper.x - gm->player->aabb.lower.x >= 0.5f) {
		gm->player->aabb.lower.x += cliping;
		gm->player->aabb.upper.x -= 0.2f;
	}
	if (gm->player->aabb.upper.z - gm->player->aabb.lower.z >= 0.5f) {
		gm->player->aabb.lower.z += cliping;
		gm->player->aabb.upper.z -= 0.2f;
	}
}
void AutoPhase::onDisable() {
	if (Game.getLocalPlayer() == nullptr) return;
	if (Game.getLocalPlayer()->aabb.upper.x - Game.getLocalPlayer()->aabb.lower.x <= 0.5f) {
		Game.getLocalPlayer()->aabb.lower.x -= cliping;
		Game.getLocalPlayer()->aabb.upper.x += cliping;
	}
	if (Game.getLocalPlayer()->aabb.upper.z - Game.getLocalPlayer()->aabb.lower.z <= 0.5f) {
		Game.getLocalPlayer()->aabb.lower.z -= cliping;
		Game.getLocalPlayer()->aabb.upper.z += cliping;
	}
}