// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"
#include <algorithm>
#include <numbers>  // For std::numbers::pi_v
#include <cmath>
class CrystalStruct {
public:
	float TgDameTake;
	float LpDameTake;
	Actor* targetActor;

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
		const Vec3<float> predictedPos = target->getHumanPos().add(target->stateVectorComponent->velocity);
		return std::clamp(predictedPos.distanceTo(crystalPos) / explosionRadius, 0.0f, 1.0f);
	}

	float computeImpact(const Vec3<float>& crystalPos, Actor* target, float distPercent) {
		return (1.0f - distPercent) * mc.getLocalPlayer()->dimension->blockSource->getSeenPercent(crystalPos, *target->getAABB());
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
				armorPoints += armor->getItemPtr()->getArmorValue();
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

class CrystalPlacement : public CrystalStruct {
public:
	Vec3<int> placePos;
	CrystalPlacement(Vec3<int> blockPos, Actor* target) {
		auto* lp = mc.getLocalPlayer();
		placePos = blockPos;
		auto crystalPos = blockPos.toFloat().add(0.5f, 1.f, 0.5f);
		TgDameTake = computeExplosionDamage(crystalPos, target);
		LpDameTake = computeExplosionDamage(crystalPos, lp);
		targetActor = target;
	}
};

class CrystalBreaker : public CrystalStruct {
public:
	Actor* crystalActor;
	CrystalBreaker(Actor* endCrystalActor, Actor* target) {
		auto* lp = mc.getLocalPlayer();
		crystalActor = endCrystalActor;
		auto crystalPos = *endCrystalActor->getPosition();
		TgDameTake = computeExplosionDamage(crystalPos, target);
		LpDameTake = computeExplosionDamage(crystalPos, lp);
		targetActor = target;
	}
};

class AutoCrystal : public Module {
private:
	std::string names;
	std::vector<Actor*> formodnames;
	std::vector<CrystalPlacement> placeList;
	std::vector<CrystalBreaker> breakList;
	std::vector<Actor*> entityList;
	std::vector<Actor*> targetList;
	int highestID = -1;
	bool shouldChangeID = false;
	bool Mob = false;
	bool placerot = false;
	bool breakrot = false;
	bool placing = false;
	bool breaking = false;
	Vec2<float> rotAnglePlace{};
	Vec2<float> rotAngleBreak{};
	std::mutex breakListMutex;
	std::mutex placeListMutex;
	int Ticks = 20;
public:
	float targetRange = 12.f;

	bool autoPlace = true;
	float placeRange = 7.f;
	float crystalRange = 3.f;
	float minPlaceDame = 4.5f;
	float maxPlaceDame = 6.f;
	int multiPlace = 1;
	int placeDelay = 1;

	bool autoBreak = true;
	float breakRange = 6.f;
	float minBreakDame = 4.5f;
	float maxBreakDame = 6.f;
	int breakDelay = 2;
	bool idPredict = false;
	int packets = 1;
	int sendDelay = 1;
private:
	int placeDelayTick = 0;
	int breakDelayTick = 0;
	int sendDelayTick = 0;
	void populateTargetList(LocalPlayer* localPlayer, Level* level);
	void handleTargetDistance(LocalPlayer* localPlayer);
	std::string formatNames(Actor* target);
	void handleCrystalSwitching(PlayerInventory* plrInv);
	void collectTargets(std::vector<Actor*>& targetList, Actor* localPlayer, float targetRange);
	void handleCrystalActions(LocalPlayer* localPlayer, std::vector<Actor*>& targetList, bool autoPlace, bool autoBreak, bool idPredict);
protected:
	static bool sortEntityByDist(Actor* a1, Actor* a2);
	static bool sortCrystalByTargetDame(CrystalStruct a1, CrystalStruct a2);
	bool isHoldingCrystal();
	bool canPlaceCrystal(Vec3<int> placePos);
	void generatePlacement(Actor* target);
	void getCrystalActorList(Actor* target);
	void placeCrystal();
	void breakCrystal();
	void cleardalist();
	//void clearLists();
	void breakIdPredictCrystal();
public:
	// private int prevCrystalsAmount, crystalSpeed, invTimer;
	Actor* currenttarget = nullptr;
	int prevCrystalsAmount;
	int crystalSpeed;
	int invTimer;
	bool Crystalcounter = false;
	AutoCrystal();
	//virtual void onRender(MinecraftUIRenderContext* renderCtx);
	virtual std::string getModName() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onNormalTick(Actor* actor) override;
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
	//virtual void onRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onImGuiRender(ImDrawList* d) override;
};