#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"
class AutoObsidian : public IModule {
private:
	bool aovcl = false;
	void findObsi();
//	vec3_ti blockpos2 = { 0,0,0 };
	int slotab = 0;
public:
	bool silentab = false;
	float range = 7;

	bool tryAutoTrap2(vec3_ti blkPlacement);
	bool canpla(vec3_ti blockPos);

	AutoObsidian();

	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
//	virtual void onPlayerTick(C_Player* plr) override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
//	virtual void onEnable() override;
};