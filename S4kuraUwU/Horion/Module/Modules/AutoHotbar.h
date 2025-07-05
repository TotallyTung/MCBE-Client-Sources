#pragma once
#include "Module.h"

class AutoHotbar : public IModule {
public:
	int slot1 = 8;
	bool crystal = true;
	int slot2 = 7;
	bool Gapple = true;
	AutoHotbar();
	~AutoHotbar();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
