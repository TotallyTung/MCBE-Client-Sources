#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class AutoPhases : public IModule {
public:
	float amountX = 0.85f;
	float amountZ = 0.5f;
	SettingEnum mode = this;
	bool wasPressed = false;
	int range = 1;
	virtual const char* getRawModuleName();
	virtual void onTick(C_GameMode* gm);
	virtual void onDisable();
	AutoPhases();
};