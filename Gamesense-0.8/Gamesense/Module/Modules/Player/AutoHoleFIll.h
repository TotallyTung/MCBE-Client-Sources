#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class AutoHoleFill : public IModule {
private:
	int wallRange = 5;
	bool obsidian = true;
	bool bedrock = true;
	bool doPitchUp = true;
	int Odelay = 0;
	float delay = 0.14f;
	int blockPerSec = 20;

	bool hasStarted = false;
    SettingEnum swap = this;
    SettingEnum render = this;

public:
	int del0 = 0;
	bool disableOnStop = true;
	int origSlot;
    int slotHF = 0;
	std::vector<C_Entity*> targs;

	bool attemptPitchUp = false;
	std::vector<vec3_ti> autoholefill;

	int range = 5;
	AutoHoleFill();
	~AutoHoleFill();
	// Inherited via IModule
	virtual const char* getRawModuleName() override;
	virtual void onEnable();
	virtual void onDisable();
	virtual void onWorldTick(C_GameMode* gm) override;
	virtual void onTick(C_GameMode* gm) override;
};