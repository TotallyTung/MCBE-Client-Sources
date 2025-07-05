#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class CrystalAuraWTA : public IModule {
private:
	int delay = 0;
	bool autoplace = true;
	bool crystalCheck = true;
	bool yLock = false;
	bool isClick = false;
	bool doMultiple = true;
	bool spoof = false;

public:

	CrystalAuraWTA();
	~CrystalAuraWTA();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	int range = 7;
	bool BadMan = true;
	vec3_t espPosUpper;
	vec3_t espPosLower;
	std::vector<vec3_ti> toPlace;
};
#pragma once
