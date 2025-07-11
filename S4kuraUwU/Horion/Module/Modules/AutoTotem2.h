#pragma once
#include "Module.h"

class AutoTotem2 : public IModule {
public:
	AutoTotem2();
	~AutoTotem2();
	SettingEnum Mode;
	bool setOffhand = false;

	virtual void craftingScreenController_tick(C_CraftingScreenController* c);
	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
	virtual void onTick(C_GameMode* gm) override;
};