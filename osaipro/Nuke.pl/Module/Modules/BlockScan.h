#pragma once
#include "../ModuleManager.h"
#include "../../DrawUtils.h"
#include "Module.h"

class BlockScan : public IModule {
private:
	float width = 0.5;
	int range = 5;
public:
	virtual void onLevelRender();
	virtual void onTick(GameMode* gm) override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx);
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx);
	virtual const char* getModuleName();
	BlockScan();
};