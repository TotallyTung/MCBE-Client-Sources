#pragma once
#pragma once
#include "../../../Config/ConfigManager.h"
#include "../../../Config/SettingManager.h"
#include "../Module.h"

class AutoConfig : public IModule {
public:
	int place = 0;
	bool AutoSave = false;

	AutoConfig();
	~AutoConfig();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};