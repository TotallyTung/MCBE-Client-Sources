#pragma once
#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class GravityAura : public IModule {
private:
	SettingEnum block = this;

public:
	int placeRange = 5;
	int pillarHeight = 3;
	int blockHeight = 3;
	bool Bypass = false;
	bool multiplace = false;
	bool onClick = false;
	bool tryAutoFallBlock(vec3_t AutoFallBlock);
	GravityAura();
	~GravityAura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
};