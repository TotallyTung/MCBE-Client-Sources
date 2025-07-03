#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class NukeSpeed : public IModule {
private:
	float speed = 0.325f;
	bool ssp = false;
	float sneakspeed = 1.f;
	float speedreal = 0.5f;
	float timers = 24.f;
public:
	bool DamageBoosts = false;
	float boosttimer = 24.f;
	int boostmaxtick = 22;
	int boostmintick = 22;
	float boostspeed = 1.3f;
	int nowtick = 0;
	bool attack = false;
	NukeSpeed();
	~NukeSpeed();

	// Inherited via IModule
	virtual void onTick(GameMode* gm) override;
	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onMove(MoveInputHandler* input) override;
};
