#pragma once
#include "Module.h"

class CrystalStruct {
public:
	float TgDameTake;
	float LpDameTake;
	Entity* targetActor;
protected:
	float computeExplosionDamage(const Vec3& crystalPos, Entity* tg) {
		static const float explosionRadius = 12.f;  // 6 * 2
		Vec3 pos = tg->getHumanPos();
		float dist = pos.dist(crystalPos) / explosionRadius;
		int armorPoints = 0;
		float epf = 0.f;
		if (dist <= 1.f) {
			float exposure = Game.getLocalPlayer()->region->getSeenPercent(crystalPos, tg->aabb);
			float impact = (1 - dist) * exposure;
			// return ((impact * impact * 3.5f + impact * 0.5f * 7.f) * explosionRadius + 1.f);
			float dame = ((impact * impact * 3.5f + impact * 0.5f * 7.f) * explosionRadius + 1.f);
			for (int i = 0; i < 4; i++) {
				ItemStack* armor = tg->getArmor(i);
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
class CrystalPlacements : public CrystalStruct {
public:
	Vec3i placePos;
	CrystalPlacements(const Vec3i& blockPos, LocalPlayer* lp, Entity* tg) {
		BlockSource* region = lp->region;
		placePos = blockPos;
		Vec3 crystalPos = blockPos.toVec3t().add(0.5f, 1.f, 0.5f);
		TgDameTake = computeExplosionDamage(crystalPos, tg);
		LpDameTake = computeExplosionDamage(crystalPos, lp);
		targetActor = tg;
	}
};
class CrystalBreaker : public CrystalStruct {
public:
	Entity* crystalActor;
	bool isIdPredict = false;
	CrystalBreaker(Entity* endCrystalActor, LocalPlayer* lp, Entity* tg, bool idPredict = false) {
		BlockSource* region = lp->region;
		crystalActor = endCrystalActor;
		Vec3 crystalPos = endCrystalActor->eyePos0;
		TgDameTake = computeExplosionDamage(crystalPos, tg);
		LpDameTake = computeExplosionDamage(crystalPos, lp);
		targetActor = tg;
		isIdPredict = idPredict;
	}
};
class AutoCrystal : public IModule {
private:
	std::vector<CrystalPlacements> placeList;
	std::vector<CrystalBreaker> breakList;
public:
	std::vector<Entity*> entityList;
	std::vector<Entity*> targetList;
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
	int sendDelay = 1;
	//Switch
	SettingEnum switchMode = SettingEnum(this);
	SettingEnum spoofMode = SettingEnum(this);
	//Others
	bool stopOnEat = true;

private:
	bool shouldSwitchBack = false;
	int oldSlot = 0;

	int placeDelayTick = 0;
	int breakDelayTick = 0;
	int sendDelayTick = 0;

protected:
	inline bool isHoldingCrystal();
	bool canPlaceCrystal(BlockSource* blockSource, const Vec3i& placePos, Entity* tg);
	int getCrystalSlot(LocalPlayer* locaPlayer);
	void generatePlacement(LocalPlayer* localPlayer, Entity* target);
	void placeCrystal(GameMode* gm);
	void getCrystalActorList(LocalPlayer* localPlayer, Entity* target);
	void breakCrystal(GameMode* gm);
	void breakIdPredictCrystal(GameMode* gm);
	static void findEntity(Entity* currentEntity, bool isRegularEntity);
	static bool sortCrystalByTargetDame(CrystalStruct a1, CrystalStruct a2);
	static bool sortEntityByDist(Entity* a1, Entity* a2);

public:
	AutoCrystal();

	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onSendPacket(Packet* packet) override;
	virtual void onLevelRender() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
};