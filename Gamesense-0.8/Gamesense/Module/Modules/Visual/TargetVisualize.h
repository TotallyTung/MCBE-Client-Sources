#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"
class TargetVisualize : public IModule {
public:
	int range = 10;
	virtual void onLevelRender();
	virtual const char* getRawModuleName();
	TargetVisualize();
};