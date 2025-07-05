#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class HoleFiller : public IModule {
private:
	int wallRange = 5;
	bool obsidian = true;
	bool bedrock = true;
	bool holeSwitch = true;
	bool doPitchUp = true;
	int Odelay = 0;
	float delay = 0.14f;
	int blockPerSec = 20;

	bool hasStarted = false;

public:
	int del0 = 0;
	int origSlot;

	bool attemptPitchUp = false;
	std::vector<vec3_ti> plugMeDaddyUWU;

	int range = 5;
	HoleFiller();
	~HoleFiller();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable();
	virtual void onDisable();
	virtual void onTick(C_GameMode* gm) override;
};