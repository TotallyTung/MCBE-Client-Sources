#pragma once
#include "../Module.h"

class Killaura : public Module {
private:


	int Mode = 0;

	int switchMode = 0;
	bool isInRange = false;
	bool hurttime = false;
	int attackDelay = 2;
	int mult = 1;
	int attackDelayTick = 0;
	bool visualRange = false;
	UIColor vRColor = UIColor(255, 255, 255, 255);
	bool targetVisualize = false;
	UIColor visualizeColor = UIColor(255, 255, 255, 255);

	int getBestWeaponSlot();
public:
	float TargetDis = 0;

	float ARange = 5.f;
	int timer = 20;
	int APS = 20;
	Vec2<float> joe;
	bool noSwing;
	float MaxSrange = 20.f;
	float MinSrange = 3.f;
	float targetRange = 7.f;
	bool attackMob = false;
	std::vector<Actor*> targetList;
	Vec2<float> rotAngle;
	Killaura();

	std::string getModName();

	int rotMode = 1;
	virtual void onNormalTick(Actor* actor) override;
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
	virtual void onDisable() override;
};