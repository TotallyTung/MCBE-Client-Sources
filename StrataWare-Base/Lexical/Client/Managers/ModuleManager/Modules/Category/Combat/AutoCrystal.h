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
	static float getExplosionDamage(const Vec3<float>& blockPos, Actor* _actor) {
		return DamageUtils::getExplosionDamage(blockPos, _actor);
	}
};

class CrystalPlace : public CrystalData {
public:
	BlockPos blockPos;
	CrystalPlace(Actor* _actor, const BlockPos& _blockPos) {
		blockPos = _blockPos;

		Vec3<float> crystalPos = Vec3<float>(
			static_cast<float>(_blockPos.x) + 0.5f,
			static_cast<float>(_blockPos.y) + 1.0f,
			static_cast<float>(_blockPos.z) + 0.5f
		);

		targetDamage = getExplosionDamage(crystalPos, _actor);
		localDamage = getExplosionDamage(crystalPos, Game::ourPlayer());
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
		targetDamage = getExplosionDamage(crystalPos, _actor);
		localDamage = getExplosionDamage(crystalPos, Game::ourPlayer());
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
	bool isPlaceValid(const BlockPos& blockPos, Actor* actor);
	void generatePlacement(Actor* actor);
	void getCrystals(Actor* actor);
protected:
	void placeCrystal(GameMode* gm);
	void breakCrystal(GameMode* gm);
public:
	AutoCrystal();
	virtual void onNormalTick(LocalPlayer* localPlayer) override;
	virtual bool onSendPacket(Packet* packet) override;
	virtual void onLevelRender() override;
};