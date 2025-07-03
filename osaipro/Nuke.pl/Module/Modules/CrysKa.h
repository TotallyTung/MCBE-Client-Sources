#pragma once

#include "Module.h"
#include "../ModuleManager.h"
#include "../../../Utils/Target.h"

class CrystalKa : public IModule
{
private:
	bool isMulti = true;
	int delay = 0;
	int Odelay = 0;
	bool autoweapon = false;
	void findWeapon();
	bool silent = true;

public:
	bool targetListEmpty = false;
	Vec2 angle;
	bool isMobAura = false;
	bool hurttime = true;
	float range = 6;


	CrystalKa();
	~CrystalKa();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onPlayerTick(Player* plr);
	virtual void onEnable() override;
	virtual void onJoiningServer() override;
	virtual void onSendPacket(Packet* packet) override;
};