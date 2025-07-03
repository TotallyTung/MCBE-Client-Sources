#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Timer : public IModule
{
public:
	std::string name = "Timer";
	int timer = 50;
	bool showTps = true;
	int minTps = 20;
	int maxTps = 50;
	float speedShift = 20;
	float duration = 0.5f;
	SettingEnum mode;
	// bool tmhold = false;

	virtual const char *getRawModuleName();
	virtual const char *getModuleName() override;
	virtual void onTick(C_GameMode *gm) override;
	// virtual bool isHoldMode() override;
	virtual void onDisable() override;
	Timer();
};
