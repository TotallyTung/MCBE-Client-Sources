#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"

class CrystalStruct {
public:
	float TgDameTake;
	float LpDameTake;
	C_Entity* targetActor;
	struct CrystalPlacements {
		vec3_t toPlace;
		float enemyDmg = 0.f;
		float selfDmg = 0.f;
	};
	struct CrystalInfo {
		CrystalPlacements CPlacements;
		C_Entity* ent;
	};

protected:
	float computeExplosionDamage(const vec3_t& crystalPos, C_Entity* tg) {
		static const float explosionRadius = 12.f;  // 6 * 2
		vec3_t pos = tg->getHumanPos();
		float dist = pos.dist(crystalPos) / explosionRadius;
		int armorPoints = 0;
		float epf = 0.f;
		if (dist <= 1.f) {
			float exposure = g_Data.getLocalPlayer()->region->getSeenPercent(crystalPos, tg->aabb);
			float impact = (1 - dist) * exposure;
			// return ((impact * impact * 3.5f + impact * 0.5f * 7.f) * explosionRadius + 1.f);
			float dame = ((impact * impact * 3.5f + impact * 0.5f * 7.f) * explosionRadius + 1.f);
			for (int i = 0; i < 4; i++) {
				C_ItemStack* armor = tg->getArmor(i);
				if (armor->isValid()) {
					armorPoints += armor->getItem()->getArmorValue();
					epf += 5;
				}
			}
			dame -= dame * armorPoints * 0.035f;                                                  // 0.04f
			dame -= dame * std::min(ceilf(std::min(epf, 25.f) * (75.f / 100.f)), 20.f) * 0.035f;  // 0.04f  // enchant reduction
			return dame;
		}
		return 0.f;
	}
};
struct lineResults {
	float blockCount = 0;
	vec3_t lastSolidBlock;
};
inline lineResults countBlksAlongLine(vec3_t start, vec3_t end) {
	vec3_t endf = end.floor();
	vec3_t startf = start.floor();

	lineResults rtn;

	if (endf == startf) {
		rtn.blockCount = 0.f;
		rtn.lastSolidBlock = startf;
		return rtn;
	}

	float dist = startf.dist(endf);

	float dX = endf.x - startf.x;
	float dY = endf.y - startf.y;
	float dZ = endf.z - startf.z;

	int steps = 200;
	const float segment = dist / steps;
	const vec3_t seggs3d = vec3_t((dX / steps), (dY / steps), (dZ / steps));

	vec3_t imHere = startf;
	vec3_ti inCoord = startf;

	while (steps-- >= 0) {
		inCoord = imHere.floor();

		if (!g_Data.getLocalPlayer()->region->getBlock(inCoord)->blockLegacy->material->isReplaceable) {
			rtn.blockCount = rtn.blockCount + segment;
			rtn.lastSolidBlock = inCoord.toVector3();
		}
		imHere = imHere.add(seggs3d);
	}
	return rtn;
}
class CrystalPlacements : public CrystalStruct {
public:
	vec3_ti placePos;
	CrystalPlacements(const vec3_ti& blockPos, C_LocalPlayer* lp, C_Entity* tg) {
		C_BlockSource* region = lp->region;
		placePos = blockPos;
		vec3_t crystalPos = blockPos.toVector3().add(0.5f, 1.f, 0.5f);
		TgDameTake = computeExplosionDamage(crystalPos, tg);
		LpDameTake = computeExplosionDamage(crystalPos, lp);
		targetActor = tg;
	}
};
class CrystalBreaker : public CrystalStruct {
public:
	C_Entity* crystalActor;
	bool isIdPredict = false;
	CrystalBreaker(C_Entity* endCrystalActor, C_LocalPlayer* lp, C_Entity* tg, bool idPredict = false) {
		C_BlockSource* region = lp->region;
		crystalActor = endCrystalActor;
		vec3_t crystalPos = endCrystalActor->eyePos0;
		TgDameTake = computeExplosionDamage(crystalPos, tg);
		LpDameTake = computeExplosionDamage(crystalPos, lp);
		targetActor = tg;
		isIdPredict = idPredict;
	}
};
class AutoCrystal2 : public IModule {
private:
	std::vector<CrystalPlacements> placeList;
	std::vector<CrystalBreaker> breakList;

public:
	std::vector<C_Entity*> entityList;
	std::vector<C_Entity*> targetList;
	int highestID = -1;
	bool shouldChangeID = false;

public:
	float targetRange = 12.f;
	// Place
	bool autoPlace = true;
	float placeRange = 7.f;
	float crystalRange = 3.f;
	float minPlaceDame = 4.5f;
	float maxPlaceDame = 6.f;
	int multiPlace = 1;
	int placeDelay = 1;
	// Break
	bool autoBreak = true;
	float breakRange = 6.f;
	float minBreakDame = 4.5f;
	float maxBreakDame = 6.f;
	int breakDelay = 2;
	bool idPredict = false;
	int packets = 1;
	float R = 0.25f;
	float B = 0.25f;
	float G = 0.25f;
	float alpha2 = 0.25f;
	float alpha = 0.25f;
	int sendDelay = 1;
	// Switch
	SettingEnum switchMode = SettingEnum(this);
	SettingEnum spoofMode = SettingEnum(this);
	// Others
	bool stopOnEat = true;

private:
	bool shouldSwitchBack = false;
	int oldSlot = 0;

	int placeDelayTick = 0;
	int breakDelayTick = 0;
	int sendDelayTick = 0;

protected:
	inline bool isHoldingCrystal();
	bool canPlaceCrystal(C_BlockSource* blockSource, const vec3_ti& placePos, C_Entity* tg);
	int getCrystalSlot(C_LocalPlayer* locaPlayer);
	void generatePlacement(C_LocalPlayer* localPlayer, C_Entity* target);
	void placeCrystal(C_GameMode* gm);
	void getCrystalActorList(C_LocalPlayer* localPlayer, C_Entity* target);
	void breakCrystal(C_GameMode* gm);
	void breakIdPredictCrystal(C_GameMode* gm);
	static void findEntity(C_Entity* currentEntity, bool isRegularEntity);
	static bool sortCrystalByTargetDame(CrystalStruct a1, CrystalStruct a2);
	static bool sortEntityByDist(C_Entity* a1, C_Entity* a2);

public:
	AutoCrystal2();

	virtual const char* getModuleName() override;
	virtual const char* getModName();
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(C_Packet* packet) override;
	virtual void onLevelRender() override;
	//virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};