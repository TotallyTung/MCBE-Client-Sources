#pragma once

#include "Module.h"
#include "../ModuleManager.h"

class APVPDisabler : public IModule {
private:
	bool packetLook = false;
	int angle = 270;
	bool onlyOnBuildMods = true;
	bool pauseOnXP = true;
	bool pauseOnBow = true;
	bool pauseOnMine = true;

public:
	APVPDisabler();
	~APVPDisabler();

	virtual const char* getModuleName() override;
	virtual void onPlayerTick(C_Player* player) override;
};