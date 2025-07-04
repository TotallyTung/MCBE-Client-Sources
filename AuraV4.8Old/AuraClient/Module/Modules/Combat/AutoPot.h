#pragma once
#include"../Module.h"
#include "../../ModuleManager.h"

class AutoPot : public IModule {
public:
	int start = 1;
	int slot = 9;
	std::string name = "AutoPot+";

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	AutoPot();
};
