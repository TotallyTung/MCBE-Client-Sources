#include "PhaseForAntiAnvil.h"

PhaseForAntiAnvil::PhaseForAntiAnvil() : IModule(0x0, Category::PLAYER, "Walk through walls") {}

PhaseForAntiAnvil::~PhaseForAntiAnvil() {
}

const char* PhaseForAntiAnvil::getModuleName() {
	return ("Phase+");
}

void PhaseForAntiAnvil::onTick(GameMode* gm) {
	gm->player->aabb.upper.y = gm->player->aabb.lower.y;
}

void PhaseForAntiAnvil::onDisable() {
	if (Game.getLocalPlayer() != nullptr)
		Game.getLocalPlayer()->aabb.upper.y += 1.8f;
}
