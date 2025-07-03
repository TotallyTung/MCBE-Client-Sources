#pragma once

#include "../../ModuleBase/Module.h"

class AutoClicker : public Module {
private:
	int delay = 0;
	int Odelay = 0;
	bool weapons = true;
	bool breakBlocks = true;
	bool rightclick = false;
	bool hold = false;

public:
	AutoClicker();

	// Inherited via IModule
	virtual void onNormalTick(LocalPlayer* gm) override;
};
