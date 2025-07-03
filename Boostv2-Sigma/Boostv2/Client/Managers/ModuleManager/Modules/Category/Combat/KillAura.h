#pragma once
#include "../../ModuleBase/Module.h"

class KillAura : public Module {
private:
	float range = 5.f;
	float headspeed = 5.f;
	float wallRange = 3.f;
	float predictvalue = 3.f;
	float test = 3.f;
	float dddd = 3.f;
	float typeP = 3.f;
	float randomizerange = 7.f;
	float valueChange = 3.f;
	float changing = 3.f;
	int interval = 1;
	int rotMode = 1;
	bool randomize = true;
	int hitType = 0;
	int hitChance = 100;
	int multiplier = 1;
	int autoWeaponMode = 0;
	bool includeMobs = false;
	bool hurtTimeCheck = false;
	bool targetVisualize;
	UIColor visualizeColor = UIColor(0, 255, 255);
private:
	std::vector<Actor*> targetList;
	int oTick = INT_MAX;
	bool shouldRot = false;
	Vec2<float> rotAngle;
	Vec2<float> rotAngle2;
	Vec2<float> rotAngle5;
	Vec2<float> ange;

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
	void onD2DRender() override;
};