#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class AutoCity : public IModule {
private:
	struct AutoCityTarg {
		vec3_ti breaker;
		C_Entity* ent;
		vec3_t entPos;
		bool shouldStart = false;
		int mineTime;
	};

	bool onedot12 = true;
	bool turnOnCA = true;
	float breakRange = 10.f;

	bool showExposed = true;
	bool inclSelf = true;

	int addtWait = 5;

public:
	float entRange = 10.f;

	int ticksToMineAC(vec3_ti toMine);

	AutoCityTarg cityTarg;

	bool isBlockGoodForBreaking(vec3_ti* blk, vec3_ti* personPos);

	AutoCity();
	~AutoCity();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onWorldTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};