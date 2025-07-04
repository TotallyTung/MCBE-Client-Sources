#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"
class FastEat : public IModule {
private:
	bool TimerWasEnabled = false;
public:
	int duration = 5;
	int timer = 35;
	SettingEnum Mode = this;
	std::string name = "FastEat";

	FastEat();
	~FastEat();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual const char* getRawModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
