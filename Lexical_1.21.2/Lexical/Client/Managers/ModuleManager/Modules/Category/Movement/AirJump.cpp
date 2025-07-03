#include "AirJump.h"

AirJump::AirJump() : Module("AirJump", "Allow you to jump even you're not on the ground", Category::MOVEMENT) {
}

void AirJump::onLevelTick(Level* level) {
	Game::getLocalPlayer()->setIsOnGround(true);
}