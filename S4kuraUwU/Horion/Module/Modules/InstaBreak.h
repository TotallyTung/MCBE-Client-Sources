#pragma once
#include "Module.h"
#include "../ModuleManager.h"


class InstaBreak : public IModule {
private:
	int addtWait = -125;
	int defMineTime = -125;
	int pickTime = 40;

	int mineTime = -125;

public:
	int mineRange = 8;
	
	InstaBreak();
	~InstaBreak();

	int hDel = 0;

	bool mineNow = false;

	int displayBlock = 0;
	int mDel = 0;
	int untilMine = 0;
	bool hasBlock = false;
	vec3_ti currentBlock;

	int ticksToMine(vec3_ti b);
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onWorldTick(C_GameMode* gm) override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};

