#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Sprint : public IModule {
public:
	bool useSprint = true;
	Sprint() : IModule(0, Category::MOVEMENT, "Automatically sprints") {
	}

	virtual const char* getRawModuleName() { return "Sprint"; }

	void onTick(C_GameMode* gm) {
		auto player = g_Data.getLocalPlayer();
		if (player == nullptr) return;

		C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
		if (!player->isSprinting() && player->velocity.magnitudexz() > 0.01f && useSprint) {
			if (GameData::isKeyDown(*input->forwardKey) && !GameData::isKeyDown(*input->sneakKey))
				player->setSprinting(true);
		}
	}
};