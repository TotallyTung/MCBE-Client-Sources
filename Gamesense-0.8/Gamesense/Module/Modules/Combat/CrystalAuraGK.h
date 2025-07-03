#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class CrystalAuraGK : public IModule
{
public:
	const char* name = "AutoCrystal";
	bool obsidianGK = false;
	int origSlot;
	int placeDelay = 0;
	int breakDelay = 0;
	bool autoplace = true;
	bool crystalCheck = true;
	bool safety = true;
	bool ignoreEat = false;
	bool stopOnTool = false;
	int maxProximity = 4;
	int targetRange = 7;
	float breakHealth = 5.f;
	float placeHealth = 5.f;
	bool yLock = false;
	bool isClick = false;
	bool doMultiple = true;
	bool ReturnOnEat = true;
	bool stopOnInv = false;
	int OBdelay = 0;
	int breakRange = 6;
	bool shouldBreak = true;
	int rad = 2;
	int slotGK = 0;
	float OutOpacity = 1.f;
	float Opacity = 1.f;
	bool vertex = false;
	SettingEnum renderType;
	SettingEnum placement;
	SettingEnum switchType;

	CrystalAuraGK();
	~CrystalAuraGK();

	// Inherited via IModule
	virtual const char *getRawModuleName() override;
	virtual const char* getModName() override;
	virtual void onTick(C_GameMode *gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	int range = 7;
	// bool BadMan = true;
	virtual void onPreRender(C_MinecraftUIRenderContext *renderCtx) override;
	vec3_t espPosUpper;
	vec3_t espPosLower;
};
