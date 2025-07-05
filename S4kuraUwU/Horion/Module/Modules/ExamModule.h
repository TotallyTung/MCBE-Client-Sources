#pragma once
#include "Module.h"
#include "../ModuleManager.h"

class ExamModule : public IModule {
private:
	int delay = 0;
	bool tryFindId = true;
	bool tryLookEnt = true;

public:
	bool SELF_getHealth = false;
	bool OTHERS_getHealth = false;

	bool rayTraceTest = false;
	//int countBlksAlongLine(vec3_t start, vec3_t end);  // wee woo raytracing

	// crystalaura testing kit
	bool testCrystalDamage = false;
	bool testPlaceSuitable = false;
	bool testDTIW = false;

	bool testLook = false;
	bool testUnlookableEnt = false;

	int pointedEnt;
	bool seen;
	SettingEnum calcDmgType;  // What method is used to calculate damage (java, bedrock)

	ExamModule();
	~ExamModule();

	// T H E  H O L Y  T R I N I T Y
	float computeExplosionDamage(vec3_t crystalPos, C_Entity* target, C_BlockSource* reg);
	float getBlastDamageEnchantReduction(C_ItemStack* armor);
	float calculateBlockDensity(C_Entity* targetEntity, vec3_t explosionPosition);

	bool isPlaceValid(vec3_t location);

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};
