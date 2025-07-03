#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

// JTWD yo
struct CrystalPlacements
{
	vec3_t toPlace;
	float enemyDmg = 0.f;
	float selfDmg = 0.f;
};

struct CrystalInfo
{
	CrystalPlacements CPlacements;
	C_Entity* ent;
};

struct lineResults
{
	float blockCount = 0;
	vec3_t lastSolidBlock;
};

inline bool checkCornerHitboxCollision2(const vec3_t& block, C_Entity* ent, bool extraCheck = false) {
	if (extraCheck) {
		if (g_Data.getLocalPlayer()->region->getBlock(block)->toLegacy()->material->isReplaceable)
			return false; // if the block isnt solid
	}

	AABB blockHitbox = AABB(block, block.add(vec3_t(1, 1, 1))); // idk why but this errors if i dont add a vec3

	if (ent->aabb.intersects(blockHitbox))
		return true;

	return false;
}

inline lineResults countBlksAlongLine(vec3_t start, vec3_t end)
{
	vec3_t endf = end.floor();
	vec3_t startf = start.floor();

	lineResults rtn;

	if (endf == startf)
	{
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

	while (steps-- >= 0)
	{
		inCoord = imHere.floor();

		if (!g_Data.getLocalPlayer()->region->getBlock(inCoord)->blockLegacy->material->isReplaceable)
		{
			rtn.blockCount = rtn.blockCount + segment;
			rtn.lastSolidBlock = inCoord.toVector3();
		}
		imHere = imHere.add(seggs3d);
	}
	return rtn;
}

class AutoCrystalRewrite : public IModule
{
private:
	vec3_t toPlace;
public:
	std::vector<CrystalInfo> RewritePlaceArr;
	const char* name = "AutoCrystalRewrite";
	vec2_t rotAngle;
	// Bools

	bool placeArrEmpty = false;
	bool fill = true;
	bool currentlySwitchingHotbar = false;
	bool rotUpNow = false;
	bool cbWasEnabled = false;
	bool rangesec;
	bool placesec;
	bool safesec;
	bool shouldBreak = false;
	bool breakArg = false;
	bool safetyFirst = false;
	bool ignoreEat = false;
	bool placeFast = false;
	bool ReturnOnEat = true;
	bool attackMulti = false;
	bool drawEnemDmg = true;
	bool synced = false;
	bool hasBroken = false;
	bool hasPlaced = true;
	// Integers

	int delay = 2;
	int breakDelay = 0;
	int amount = 1;
	int placetimes = 1;
	int maxProximity = 4;
	int OBdelay = 0;
	int origSlot;
	int ctr = 0;
	int slotMC = 0;

	// Floats

	float range = 8;
	float placeRange = 6.f; 
	float breakRange = 6.f;
	float thruWallsR = 6;
	float breakWall = 6;
	float postWallsR = 10;
	float minHealth = 5.f;
	float breakHealth = 5.f;
	float maxSelfDmg = 5.f;
	float minEnemDmg = 10.f;
	float fpThresh = 20.f;
	float dmgAtThresh = 3.f;
	float dmg_nukkit = false;
	// toggle the fill render
	float Opacity = 0.25f;	// Opacity of the fill renders
	float OutOpacity = 1.f; // Opacity of the outline renders

	// Enums

	SettingEnum facePlaceType;
	SettingEnum switchType;
	SettingEnum rotations;
	SettingEnum renderType;
	SettingEnum rendering;
	SettingEnum breakMode;
	SettingEnum priority;
	SettingEnum protocol;
	SettingEnum rotationsMode;

	AutoCrystalRewrite();
	~AutoCrystalRewrite();

	bool isPlaceValid(vec3_t location, C_Entity* atkObj);
	// T H E  H O L Y  T R I N I T Y
	float computeExplosionDamage(vec3_t crystalPos, C_Entity* target, C_BlockSource* reg);
	float getBlastDamageEnchantReduction(C_ItemStack* armor);
	// float calculateBlockDensity(C_Entity* targetEntity, vec3_t explosionPosition);

	std::vector<CrystalPlacements> generateValidPlacements(C_Entity* target, int yOffset);

	CrystalPlacements bestPlaceOnPlane(C_Entity* target, int yOffset);

	CrystalPlacements AutoCrystalMain(C_Entity* target);

	void placeCrystals();
	void crystalBreak();
	// Inherited via IModule
	virtual const char* getRawModuleName();
	virtual const char* getModName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onWorldTick(C_GameMode* gm) override;
	// virtual void onPlayerTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onPlayerTick(C_Player* plr) override;
	virtual void onSendPacket(C_Packet* packet) override;
};
