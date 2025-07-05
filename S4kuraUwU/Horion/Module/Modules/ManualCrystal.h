#pragma once
#include "Module.h"
#include "../ModuleManager.h"


class ManualCrystal : public IModule {
private:  // nope
	int delay = 0;
	bool switchToCrystal = true;
	bool spoof = false;

public:  // nope
	ManualCrystal();
	~ManualCrystal();


	int ctr = 0;
	int origSlot = NULL;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable();
	virtual void onDisable();
};
