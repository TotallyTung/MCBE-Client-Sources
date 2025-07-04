#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class CrystalAuraGK : public IModule {
public:
	bool obsidianGK = false;
	int delay = 0;
	bool autoplace = true;
	bool crystalCheck = true;
	bool yLock = false;
	bool isClick = false;
	bool doMultiple = true;
	int rad = 2;
	bool silentGK = false;
	int slotGK = 0;
	bool renderGK = true;
	bool oldrdGK = false;


	void findObGK();

	CrystalAuraGK();
	~CrystalAuraGK();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	int range = 7;
	//bool BadMan = true;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	vec3_t espPosUpper;
	vec3_t espPosLower;
};
