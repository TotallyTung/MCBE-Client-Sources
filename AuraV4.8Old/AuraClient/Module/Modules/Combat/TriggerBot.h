#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class TriggerBot : public IModule {
private:
	int APS = 11;
	int delay = 0;
	int wtfdelay = 0;
public:
	TriggerBot();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};