#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

struct CrystalPlacement {
	vec3_t toPlace;
	float enemyDmg = 0.f;
	float selfDmg = 0.f;
};

struct CrystalInfomation {
	CrystalPlacement CPlacements;
	C_Entity* ent;
};

struct lineResult {
	float blockCount = 0;
	vec3_t lastSolidBlock;
};
inline bool checkCornerHitboxCollision(const vec3_t& block, C_Entity* ent) {
	AABB targetAABB = (ent->getEntityTypeId() != 319) ?
		ent->aabb :
		AABB(ent->getHumanPos().add(-0.3f, 0, -0.3f), ent->getHumanPos().add(0.3f, 1.8f, 0.3f));

	for (int i = 0; i < (ent->getEntityTypeId() != 319 ? 4 : 8); ++i) {
		vec3_t corner = (ent->getEntityTypeId() != 319) ?
			vec3_t(i % 2 == 0 ? targetAABB.lower.x : targetAABB.upper.x,
				targetAABB.lower.y, (i < 2) ? targetAABB.lower.z : targetAABB.upper.z) :
			targetAABB.centerPoint().add(targetAABB.lower.sub(targetAABB.upper).mul(0.5f));

		if (!targetAABB.intersectsXZ(AABB(block, 1.0f, 1.0f, 1.0f))) {
			return false; // No intersection, safe to place
		}
	}

	return true; // Intersection detected, cannot place
}

inline lineResult countBloksAlongLine(vec3_t start, vec3_t end) {
	vec3_t endf = end.floor();
	vec3_t startf = start.floor();

	lineResult rtn;

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

class AutoCrystal : public IModule {
private:
	int origSlot;
	int ctr = 0;

	vec3_t toPlace;

	bool currentlySwitchingHotbar = false;

public:
	// Bools
	bool placeArrEmpty = false;
	bool rotUpNow = false;
	bool safetyFirst = false;
	bool breakCrystals = true;
	bool placeCrystals = true;
	bool renderText = true;
	bool ignoreEat = false;
	// Float
	float targetRange = 12.f;
	float placeRange = 6.f;
	float breakRange = 6.f;
	float thruWallsR = 6;
	float postWallsR = 10;
	float minHealth = 5.f;
	float maxSelfDmg = 5.f;
	float minEnemDmg = 10.f;
	float fpThresh = 20.f;
	float dmgAtThresh = 3.f;
	float alpha = 0.25f;

	// Int
	int placeDelay = 0;
	int breakDelay = 0;
	int maxProximity = 4;
	int increasingBreak = 0;

	// Enums
	SettingEnum priority;
	SettingEnum protocol;
	SettingEnum facePlaceType;
	SettingEnum switchType;
	SettingEnum rotate;
	SettingEnum mode;
	SettingEnum renderType;
	// Listeners // Functions
	const char* name = "AutoCrystal";
	std::vector<CrystalInfomation> Memory;
	vec2_t rotAngle;
	bool isPlaceValid(vec3_t location, C_Entity* atkObj);
	float computeExplosionDamage(vec3_t crystalPos, C_Entity* target, C_BlockSource* reg);
	float getBlastDamageEnchantReduction(C_ItemStack* armor);
	void breakCrystal();
	std::vector<CrystalPlacement> ValidPlacement(C_Entity* target, int yOffset);
	CrystalPlacement BestPlacement(C_Entity* target, int yOffset);
	CrystalPlacement Main(C_Entity* target);
	AutoCrystal();
	~AutoCrystal();
	virtual const char* getRawModuleName() override;
	virtual const char* getModName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPlayerTick(C_Player* plr) override;
	virtual void onWorldTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(C_Packet* packet);
};