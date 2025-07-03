#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class PopCounter : public IModule {
private:
	bool countyourself = true;
	bool listempty = true;

public:
	// SettingEnum mode = this;

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onEnable() override;
	PopCounter();
};