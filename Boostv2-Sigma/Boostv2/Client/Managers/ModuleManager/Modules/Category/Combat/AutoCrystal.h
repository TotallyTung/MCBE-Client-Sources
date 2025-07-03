#pragma once
#include "../../ModuleBase/Module.h"
#include "../../../ModuleManager.h"
#include "../../../../../../Utils/Minecraft/DamageUtil.h"

struct CrystalFadeStruct {
	Vec3<float> lastPos;
	float fadeTimer;
	float fadeDuration;
};

struct CrystalData {
public:
	float targetDamage;
	float localDamage;
	Actor* actor;
protected:
	float computeExplosionDamage(const Vec3<float>& crystalPos, Actor* target) {
		constexpr float explosionRadius = 12.f;
		const float distPercent = computeDistancePercentage(crystalPos, target, explosionRadius);
		if (distPercent > 1.0f) {
			return 0.0f;
		}

		const float impact = computeImpact(crystalPos, target, distPercent);
		float damage = calculateBaseDamage(impact, explosionRadius);

		const auto [armorPoints, epf] = getArmorDetails(target);
		damage = applyArmorReduction(damage, armorPoints, epf);

		return std::max(0.0f, damage);
	}

private:
	float computeDistancePercentage(const Vec3<float>& crystalPos, Actor* target, float explosionRadius) {
		const Vec3<float> predictedPos = target->getHumanPos().add(target->stateVector->velocity);
		return std::clamp(predictedPos.dist(crystalPos) / explosionRadius, 0.0f, 1.0f);
	}

	float computeImpact(const Vec3<float>& crystalPos, Actor* target, float distPercent) {
		return (1.0f - distPercent) * Game.getClientInstance()->getRegion()->getSeenPercent(crystalPos, target->getAABB());
	}

	float calculateBaseDamage(float impact, float explosionRadius) {
		return ((impact * impact * 3.5f + impact * 0.5f * 7.0f) * explosionRadius + 1.0f);
	}

	std::pair<int, float> getArmorDetails(Actor* target) {
		int armorPoints = 0;
		float epf = 0.0f;

		for (int i = 0; i < 4; i++) {
			auto* armor = target->getArmor(i);
			if (armor->isValid()) {
				armorPoints += armor->item->getArmorValue();
				epf += 5.0f;
			}
		}

		return { armorPoints, epf };
	}

	float applyArmorReduction(float damage, int armorPoints, float epf) {
		constexpr float armorReductionFactor = 0.035f;
		constexpr float maxEpf = 25.0f;
		constexpr float epfFactor = 0.75f;
		constexpr float maxEpfCap = 20.0f;

		damage -= damage * armorPoints * armorReductionFactor;
		damage -= damage * std::min(ceilf(std::min(epf, maxEpf) * epfFactor), maxEpfCap) * armorReductionFactor;

		return damage;
	}
};

class CrystalPlace : public CrystalData {
public:
	BlockPos blockPos;
	CrystalPlace(Actor* _actor, const BlockPos& _blockPos) {
		blockPos = _blockPos;
		Vec3<float> crystalPos = Vec3<float>(BlockPos(_blockPos).toFloat()).add((0.5f, 1.f, 0.5f));  // end crystal is entity not block :joebig:
		targetDamage = computeExplosionDamage(crystalPos, _actor);
		localDamage = computeExplosionDamage(crystalPos, Game.getLocalPlayer());
		actor = _actor;
	}
};

class CrystalBreak : public CrystalData {
public:
	Actor* crystal;
	CrystalBreak(Actor* _actor, Actor* _crystal) {
		Vec3<float> crystalPos = _crystal->getEyePos();
		actor = _actor;
		crystal = _crystal;
		targetDamage = computeExplosionDamage(crystalPos, _actor);
		localDamage = computeExplosionDamage(crystalPos, Game.getLocalPlayer());
	}
};

class AutoCrystal : public Module {
public:
	std::vector<CrystalPlace> placeList;
	std::vector<CrystalBreak> breakList;
	std::vector<Actor*> targetList;
	std::vector<Actor*> entityList;
	std::vector<CrystalFadeStruct> fadeList;
public:
	Vec3<float> lastPlaceLoc;
	std::unordered_map<int, Vec3<float>> lerpData;
	float decreasingAlpha = 0.f;
	int highestId = -1;
	bool shouldChange = false;
	int iBoostDelay = 0;
	int iPlaceDelay = 0;
	int iBreakDelay = 0;
public:
	bool place = true;
	bool explode = true;
	int pageset;
	bool multiTask = true;
	bool safety = true;
	bool java = false;
	bool antiTotem = false;
	int placeRange = 6;
	int breakRange = 6;
	int targetRange = 12;
	float proximity = 6.f;
	float enemyDamage = 4.f;
	float localDamage = 12.f;
	int wasteAmount = 1;
	bool facePlace = false;
	bool rotate = false;
	int placeSpeed = 19;
	int breakSpeed = 20;
	int predictSpeed = 20;
	bool predict = false;
	int predictPacket = 5;
	bool extrapolate = false;
	bool swap = true;
	bool switchBack = true;
	float boostDmg = 4.f;
	int renderType = 0;
	bool dmgText = false;
	bool selfTest = false;
	int animType = 0;
	float slideSpeed = 0.4f;
	float fadeDur = 3.f;
	UIColor renderColor = UIColor(255, 255, 255, 60);
protected:
	static bool sortCrystal(CrystalData c1, CrystalData c2);
	void predictEndCrystal(GameMode* gm);
	bool isPlaceValid(const BlockPos& blockPos, Actor* actor);
	void generatePlacement(Actor* actor);
	void getCrystals(Actor* actor);
protected:
	void placeCrystal(GameMode* gm);
	void breakCrystal(GameMode* gm);
public:
	AutoCrystal();
	virtual void onNormalTick(LocalPlayer* localPlayer) override;
	virtual void onSendPacket(Packet* packet) override;
	void onD2DRender() override;
};