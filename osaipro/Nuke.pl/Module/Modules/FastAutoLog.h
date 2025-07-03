#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class FastAutoLog : public IModule {
public:
	int health = 8;
	FastAutoLog() : IModule(0, Category::PLAYER, "Nigger sugiru shit") {
		registerIntSetting("Health", &health, health, 0, 20);
	};
	~FastAutoLog() {};

	// Inherited via IModule
	virtual const char* getModuleName() override { return "FastAutoLog"; };
	virtual void onTick(GameMode* gm) override {
		if (gm->player->getHealth() <= health) {Utils::sendCommand("hub");}
	};
};
