#pragma once
#include "../../../Utils/Logger.h"
#include "../../../Utils/Utils.h"
#include "../ModuleManager.h"
#include "Module.h"

class LavaPlacer : public IModule {
private:
	int range = 7;
	int dwellTime = 8;
	bool predict = true;

	int pDelay = 0;

	void placeDecisions(vec3_t placeHere);

public:
	int origSlot;

	LavaPlacer();
	~LavaPlacer();


	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};
