#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"
class AutoObsidian : public IModule {
private:
	bool aovcl = false;
	void findObsi();
	int slotab = 0;
public:
	bool silentab = false;
	const char* name = "Flatten";
	float range = 7;

	bool tryAutoTrap2(vec3_ti blkPlacement);
	bool canpla(vec3_ti blockPos);

	AutoObsidian();

	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual const char* getRawModuleName() override;
	virtual const char* getModName() override;
	virtual void onTick(C_GameMode* gm) override;
};