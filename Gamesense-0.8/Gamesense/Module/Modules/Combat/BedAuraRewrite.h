#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"
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
	vec2_t playerPos;

public:
	SettingEnum priority;
	void explodeBeds();
	void findBestPlace();
	BedAuraRewrite();
	~BedAuraRewrite();

	// Inherited via IModule
	virtual const char* getModName();
	virtual const char* getRawModuleName();
	virtual void onTick(C_GameMode* gm);
	virtual void onEnable();
	virtual void onDisable();
	int range = 7;
	const char* name = "BedAura";
	SettingEnum rotations = this;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	vec3_t espPosUpper;
	vec3_t espPosLower;
	virtual void onPlayerTick(C_Player* plr) override;
	virtual void onSendPacket(C_Packet* packet) override;
};
#pragma once
