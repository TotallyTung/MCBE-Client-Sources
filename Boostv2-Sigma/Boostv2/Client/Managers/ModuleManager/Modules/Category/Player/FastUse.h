#pragma once
#include "../../ModuleBase/Module.h"

class FastUse : public Module {
private:
	int delay = 1;
	int usePerTick = 1;
	bool onlyXP = false;

	int tickDelay = 0;
public:
	FastUse();
	void onNormalTick(LocalPlayer* localPlayer) override;
};