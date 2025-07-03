#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class Timer2 : public IModule {
public:
	int timer3 = 24;
	//bool tmhold = false;

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	//virtual bool isHoldMode() override;
	virtual void onDisable() override;
	Timer2();
};
