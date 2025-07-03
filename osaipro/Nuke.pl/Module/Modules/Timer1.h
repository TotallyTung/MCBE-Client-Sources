#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class Timer1 : public IModule {
public:
	int timer2 = 24;
	//bool tmhold = false;

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	//virtual bool isHoldMode() override;
	virtual void onDisable() override;
	Timer1();
};
