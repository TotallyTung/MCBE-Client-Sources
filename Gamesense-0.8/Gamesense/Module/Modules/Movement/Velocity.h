#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Velocity : public IModule {
public:
	float x = 0.f;
	float y = 0.f;

	std::string name = "Velocity";

	virtual const char* getRawModuleName();
	virtual const char* getModName();
	virtual void onTick(C_GameMode* gm);
	Velocity();
};