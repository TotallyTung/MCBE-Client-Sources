#pragma once

#include "../Module.h"
#include "../../ModuleManager.h"
#include "../../Utils/TargetUtil.h"

class KillauraUwU : public IModule
{
private:
	int slot;
	int n = 0;
	int APS = 20;
	void findWeapon();
	void findWeaponWater();
	void findWeaponAir();
	SettingEnum autoweapon;
	SettingEnum rotation;
	SettingEnum priority;
	SettingEnum selectmode;
	bool boost = false;
	bool swingSound = true;
	bool hurttime = false;
	bool targetESP = false;
	vec2_t localAngle = {};
	float rotationSpeed = 180.f;
	bool randomPitch = false;
	bool randomYaw = false;
	bool swing = true;

public:
	bool targetListEmpty = true;
	bool rotations = false;
	bool isMobAura = false;
	vec2_t angle;
	SettingEnum mode;
	int apsBoost = 1;
	float attackrange = 6.f;
	float swingRange = 6.f;
	bool canswing = false;

	KillauraUwU();
	~KillauraUwU();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onLevelRender()override;
	virtual void onGetPickRange();
	//virtual void onAttack(Entity* attackEnt) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend);
};
#pragma once
