#include "AutoSprint.h"

AutoSprint::AutoSprint() : Module("AutoSprint", "Automatically sprint without holding the key", Category::MOVEMENT) {
	modeEnum = (EnumSetting*)registerSetting(new EnumSetting("Mode", "NULL", { "Legit", "Omni" }, &mode, 0));
}

std::string AutoSprint::getModeText() {
	return modeEnum->enumList[mode];
}

void AutoSprint::onNormalTick(LocalPlayer* localPlayer) {
	if (!Game::canUseMoveKeys())
		return;

	bool isSprinting = localPlayer->getStatusFlag(ActorFlags::Sprinting);
	if (isSprinting)
		return;

	if (mode == 0) {
		if (Game::isKeyDown('W'))
			localPlayer->setSprinting(true);
	}
	else {
		if (Game::isKeyDown('W') || Game::isKeyDown('A') || Game::isKeyDown('S') || Game::isKeyDown('D'))
			localPlayer->setSprinting(true);
	}
}