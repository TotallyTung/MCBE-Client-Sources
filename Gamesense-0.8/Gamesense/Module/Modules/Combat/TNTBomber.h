#pragma once
#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class TNTBomber : public IModule {
private:
	int placedelay = 1;
	int ignitedelay = 3;
	bool airplace = false;

	bool hasIgnited = false;
	bool hasPlacedTNT = false;
	bool DhasPlacedTNT = false;
	bool DhasIgniteTNT = false;
	int tickTimer = 0;
	bool isHoldingTNT = false;
	int swordSlot = 0;
	int tntSlot = 0;
	bool isHoldingSword = false;
	SettingEnum swap;
	SettingEnum trap;
	bool enableTrap = false;
	vec3_t neckBreaker;

public:
	TNTBomber();
	~TNTBomber();

	int range = 10;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};