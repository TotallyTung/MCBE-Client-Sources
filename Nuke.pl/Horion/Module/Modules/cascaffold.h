#pragma once
#include "Module.h"
#include "../ModuleManager.h"
class cascaffold : public IModule {
private:
	bool aovcl = false;
	void findObsi();
//	Vec3i blockpos2 = { 0,0,0 };
	int slotab = 0;
public:
	bool silentab = false;
	float range = 5;
	bool notarget = false;

	bool tryAutoTrap2(Vec3i blkPlacement);
	bool canpla(Vec3i blockPos);

	cascaffold();

	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
//	virtual void onPlayerTick(C_Player* plr) override;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
//	virtual void onEnable() override;
};