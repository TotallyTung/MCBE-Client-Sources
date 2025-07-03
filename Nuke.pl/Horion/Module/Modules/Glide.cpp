#include "Glide.h"

Glide::Glide() : IModule(0, Category::MOVEMENT, "Float down like a chicken.") {
	registerFloatSetting("Value", &glideMod, glideMod, -2, 1);
}

Glide::~Glide() {}

const char* Glide::getModuleName() {
	return "Glide";
}
const char* Glide::getModName() {
	static char modName[30];
	snprintf(modName, 30, "%.2f", glideModEffective);
	return modName;
}

void Glide::onTick(GameMode* gm) {
	glideModEffective = glideMod;
	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();

	if (Game.canUseMoveKeys()) {
		if (GameData::isKeyDown(*input->spaceBarKey))
			glideModEffective += 0.2f;
		if (GameData::isKeyDown(*input->sneakKey))
			glideModEffective -= 0.2f;
	}
	gm->player->velocity.y = glideModEffective;
}