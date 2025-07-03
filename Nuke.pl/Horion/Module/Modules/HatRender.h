#pragma once

#include "Module.h"

class HatRender : public IModule {
public:
	HatRender();
	int test = 0;
	bool rainbow = false;
	float opacity = 0.5f;
	virtual const char* getModuleName() override;
	virtual void onLevelRender() override;
};