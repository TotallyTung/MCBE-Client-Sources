#pragma once

#include "Module.h"

class ChestStealer : public IModule {
private:
	int delay = 0;
	int setDelay = 7;
	bool dump = true;
	bool enhanced = true;

public:
	ChestStealer();
	~ChestStealer();

	// Inherited via IModule
	virtual void chestScreenController_tick(ChestScreenController* c);
	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
};
