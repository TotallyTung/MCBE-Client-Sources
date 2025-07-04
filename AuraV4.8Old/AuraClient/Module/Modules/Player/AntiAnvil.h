#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"
class AntiAnvil : public IModule {
private:
	bool caidelljthe = false;
	void findPickaxeLOL3();
	vec3_ti blockpos3 = { 0,0,0 };
	int slottd2 = 0;
public:
	bool silenttd2 = false;

	AntiAnvil();

	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onPlayerTick(C_Player* plr) override;
	virtual const char* getModuleName() override;
//	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};