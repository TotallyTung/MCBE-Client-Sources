#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class Center : public IModule {
public:
	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
	Center();
};