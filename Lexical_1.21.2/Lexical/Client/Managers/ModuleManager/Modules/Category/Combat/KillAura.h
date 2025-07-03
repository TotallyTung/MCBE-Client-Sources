#pragma once
#include "../../ModuleBase/Module.h"

class KillAura : public Module {
private:
	float range = 5.f;
	float wallRange = 3.f;
	int interval = 1;
	int multiplier = 1;
	int rotMode = 1;
	int hitType = 0;
	int hitChance = 100;
	int autoWeaponMode = 0;
	bool includeMobs = false;

	std::vector<Actor*> targetList;
	int oTick = INT_MAX;
	bool shouldRot = false;
	Vec2<float> rotationAngle;
	static bool sortByDist(Actor* a1, Actor* a2);
	int getBestWeaponSlot(Actor* target);
	void Attack(Actor* target);

public:
	KillAura();

	std::string getModeText() override;
	void onDisable() override;
	void onEnable() override;
	void onNormalTick(LocalPlayer* localPlayer) override;
	void onUpdateRotation(LocalPlayer* localPlayer) override;
	void onSendPacket(Packet* packet) override;
};
