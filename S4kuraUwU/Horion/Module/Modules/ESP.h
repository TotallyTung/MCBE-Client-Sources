#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class ESP : public IModule {
public:

	int plrR = 255;
	int plrG = 0;
	int plrB = 0;

	int mobR = 0;
	int mobG = 0;
	int mobB = 255;

	bool isMobEsp = false;
	bool is2d = false;
	bool isItemESP = false;
	ESP();
	~ESP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	//virtual void onPlayerTick(C_Player* player) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};