#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class BedAura : public IModule
{
private:
public:
	vec2_t rotAngleB;
	bool suicide;
	bool render;
	bool fastMode;
	bool targetOnly;
	float opacity;
	float minHealth = 12.f;
	int range = 3;
	int origSlot;
	int slotNukkit = 0;
	SettingEnum swap = this;
	SettingEnum rotations = this;
	C_MoveInputHandler *inputHandler = nullptr;
	BedAura();
	~BedAura();

	std::vector<vec3_ti> bedList;
	vec3_ti pos;

	// Inherited via IModule
	virtual const char *getRawModuleName() override;
	virtual void onTick(C_GameMode *gm) override;
	virtual void onDisable() override;
};
