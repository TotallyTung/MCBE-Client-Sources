#pragma once

#include "Module.h"
#include "../ModuleManager.h"

class Speed : public IModule {
private:
	bool sprint = false;
	float lavaSpeed = 0.5f;
	float lavaSneakSpeed = 0.5f;
	float airSpeed = 0.5f;
	float sneakAirSpeed = 0.5f;
	float groundSpeed = 0.5f;
	float sneakSpeed = 0.5f;
	float waterSneakSpeed = 0.5f;
	float waterspeed = 0.5f;

	float defaultSpeed;

public:
	Speed();
	~Speed();
	virtual const char* getModuleName() override;
	virtual void onMove(MoveInputHandler* input) override;
	virtual void onTick(GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};