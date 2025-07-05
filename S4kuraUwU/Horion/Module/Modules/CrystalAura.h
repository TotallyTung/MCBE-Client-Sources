#pragma once
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class CrystalAura : public IModule {
private:
	int prevSlot;
	int delay = 0;
	int delay2 = 0;
	int eRange = 3;
	bool autoplace = true;
	bool pEnhanced = true;
	bool dEnhanced = true;
	bool Preview = true;
	bool AutoSelect = true;
	bool FinishSelect = false;
	bool multiaura = false;
	bool silentswitch = false;
	bool info = false;
	C_PlayerInventoryProxy* supplies = nullptr;
	C_Inventory* inv = nullptr;
	vec3_ti getRenderPositon = vec3_ti(0, 0, 0);
	SettingEnum mode;

public:
	CrystalAura();
	~CrystalAura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	int getExplosionDamage(vec3_t pos1, vec3_t pos2) {
		int protection = 0;
		if (pos1.dist(pos2) > 12.f)
			return 69420;
		protection += pos1.dist(pos2) * 4;
		int i = pos1.dist(pos2) * 4;
		for (float n = 0.f; !(n > i); n++) {
			vec3_t pos = pos2.sub(pos1).div(i).mul(n).floor();
			C_Block* block = g_Data.getLocalPlayer()->region->getBlock(pos1.add(pos).floor());
			if (block->toLegacy()->blockId == 7 || block->toLegacy()->blockId == 49) {
				protection += 40;
			} else if (block->toLegacy()->material->isSolid) {
				protection += 20;
			}
		}

		return protection;
		return 0;
	}
	int cRange = 10;
	int pRange = 5;
	int range = 10;
	int proximity = 3;
	void CPlace(C_GameMode* gm, vec3_t* pos);
	void DestroyC(C_Entity* ent, int range);
};
#pragma once
