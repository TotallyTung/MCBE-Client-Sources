#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class CrystalAuraNukkit : public IModule {
private:
	int delay = 3;
	bool autoplace = true;
	bool crystalCheck = true;
	bool yLock = false;
	bool isClick = false;
	bool doMultiple = true;
	

public:
	CrystalAuraNukkit();
	~CrystalAuraNukkit();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	int range = 5;
	bool spoof = false;
	bool BadMan = true;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	vec3_t espPosUpper2;
	vec3_t espPosLower2;
};
