#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class BreakIndicator : public IModule {
private:

public:
	BreakIndicator();
	~BreakIndicator();
	float rSelect, gSelect, bSelect = 1;
	bool rainbow = false;
	bool line = false;
	bool filled = true;
	float fillop = 0.3;
	float lineop = 0.3;
	bool doBreaking = false;
	float thickness = 0.4f;
	bool disable = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(MinecraftUIRenderContext* ctx) override;
};
