#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class BedAuraRewrite : public IModule {
private:
	int delay = 0;
	bool autoplace = true;
	bool bedCheck = true;
	bool yLock = false;
	int slotCA = 0;
	float OutOpacity = 1.f;
	bool ignoreEat = false;
	float minHealth = 5.f;
	int bDelay = 0;
	int breakDelay = 0;
	int eRange = 5;
	bool explode = true;
	bool placeAbove = false;
	SettingEnum renderMode = this;
	SettingEnum rotationType = this;
	SettingEnum switchType = this;
	bool anvil1;
	float r1;
	float g1;
	float b1;
	float opacity1 = 0.2f;
	bool safeMode = false;
	bool instant = false;
	bool rotateToPlayer = false;
	bool rotateNow = false;
	Vec2 playerPos;

public:
	SettingEnum priority;
	void explodeBeds();
	void findBestPlace();
	BedAuraRewrite();
	~BedAuraRewrite();

	// Inherited via IModule
	virtual const char* getModuleName();
	virtual const char* getModName();
	virtual void onTick(GameMode* gm);
	virtual void onEnable();
	virtual void onDisable();
	int range = 7;
	const char* name = "BedAura";
	SettingEnum rotations = this;
	Vec3 espPosUpper;
	virtual void onLevelRender();
	virtual void onJoiningServer() override;
	Vec3 espPosLower;
	virtual void onPlayerTick(Player* plr) override;
	virtual void onSendPacket(Packet* packet) override;
};
#pragma once
