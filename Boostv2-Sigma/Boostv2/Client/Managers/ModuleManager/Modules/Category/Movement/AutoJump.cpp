#include "AutoJump.h"

AutoJump::AutoJump() : Module("AutoJump", "Jump Automaticly", Category::MOVEMENT) {
}

void AutoJump::onLevelTick(Level* level) {
	Game.getLocalPlayer()->getMoveInputHandler()->setJumping(true);
}

void AutoJump::onDisable() {
	Game.getLocalPlayer()->getMoveInputHandler()->setJumping(false);
}