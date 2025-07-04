#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"
class TargetDown : public IModule {
private:
	void findPickaxeLOL2();
	vec3_ti blockpos2 = { 0,0,0 };
	int slottd = 0;
public:
	bool silenttd = false;
	float range = 7;

	TargetDown();

	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onPlayerTick(C_Player* plr) override;
	virtual const char* getModuleName() override;
//	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};