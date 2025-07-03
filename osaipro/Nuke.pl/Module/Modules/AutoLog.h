#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class AutoLog : public IModule {
public:
	int health = 6;
	int log_health = 6;
	float interval = 15.f;
	float wait_interval = 0.f;
	SettingEnum ByeByeMessage;
	SettingEnum LogType;
	AutoLog();
	~AutoLog();
	virtual void onJoiningServer() override;
	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
	virtual void onTick(GameMode* gm) override;
};