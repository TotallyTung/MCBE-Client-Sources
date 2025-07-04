#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Timer : public IModule {
public:
	int timer = 50;
	//bool tmhold = false;

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	//virtual bool isHoldMode() override;
	virtual void onDisable() override;
	Timer();
};
