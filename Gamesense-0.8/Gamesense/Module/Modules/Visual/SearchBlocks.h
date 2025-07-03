#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class SearchBlocks : public IModule
{
private:
	float thick = 0.35f;
	float opacity = 0.35f;
	bool outline = true;
	bool chest = false;
	bool trapped_chest = false;
	bool ender_chest = false;
	bool shulker_box = false;
	bool barrel = false;
	bool dropper = false;
	bool dispenser = false;
	bool hopper = false;
	bool furnace = false;
	bool obsidian = false;
	bool shouldFill = false;
	bool shouldTracer = false;
	int range = 15;
	int Odelay = 0;
	float delay = 0.14f;

public:
	SearchBlocks();
	~SearchBlocks();

	// Inherited via IModule
	virtual const char *getRawModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext *renderCtx) override;
};