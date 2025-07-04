#include "Glide.h"

Glide::Glide() : IModule(0, Category::MOVEMENT, "Float down") {
	registerFloatSetting("value", &glideMod, glideMod, -2, 1);
}

Glide::~Glide() {
}

const char* Glide::getModuleName() {
	/*
	if (isEnabled()) {
		static char modName[30];  // This is kinda ghetto rn, there should be a better way to make this...
		sprintf_s(modName, 30, "Glide [%.2f]", glideModEffective);
		return modName;
	} else
		return ("Glide");
		*/
	//name = "Glide " + std::string(GRAY) + std::string("[") + std::string(WHITE) + std::to_string((int)glideModEffective) + std::string(".") + std::to_string((int)(glideModEffective * 10) - (int)(glideModEffective * 10));
	static char modName[30];  // This is kinda ghetto rn, there should be a better way to make this...
	sprintf_s(modName, 30, "%.2f", glideModEffective);
	//return modName;
	name = "Glide " + std::string(GRAY) + std::string("[") + std::string(WHITE) + modName + std::string(GRAY) + std::string("]");
	return name.c_str();
}

const char* Glide::getRawModuleName() {
	return "Glide";
}

void Glide::onTick(C_GameMode* gm) {
	glideModEffective = glideMod;
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (g_Data.canUseMoveKeys()) {
		if (GameData::isKeyDown(*input->spaceBarKey))
			glideModEffective += 0.2f;
		if (GameData::isKeyDown(*input->sneakKey))
			glideModEffective -= 0.2f;
	}
	gm->player->velocity.y = glideModEffective;
}
