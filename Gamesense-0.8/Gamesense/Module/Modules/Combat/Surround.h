
#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Surround : public IModule {
private:
	int delay = 0;
	bool citySwitch = true;

public:
	int slotOB = 0;

	SettingEnum type;
	bool renderSurround = true;
	bool doAirplace = false;
	//SettingEnum rotate;  // Rotations (none->obvious
						 //			  normal->clientside rotate
						 //			  silent->sends packet to go look at the crystal
						 //			  pitchUp->so that shit works on APVP

	Surround();
	~Surround();
	bool blockSwap = true;
	int prevSlot3 = -1;
	vec2_t rotAngle;
	bool pn = false;

	bool jumpDisable = false;
	bool disableOnComplete2 = true;
	bool center = true;
	bool spoof = true;
	bool buttonSwitch = true;
	void findButton();
	int slotButton = 0;
	int placeButton;
	bool placeAButton = false;
	bool isSneaked;
	SettingEnum attack;
	SettingEnum render;
	SettingEnum toggle;
	SettingEnum placement;
	float attackHealth = 12.f;
	float ropacity = 0.4f;
	const char* name = "Surround";

	std::vector<vec3_t> regularBlocks = {

		vec3_t(0, -1, 0),

		vec3_t(1, -1, 0),
		vec3_t(1, 0, 0),

		vec3_t(0, -1, 1),
		vec3_t(0, 0, 1),

		vec3_t(-1, -1, 0),
		vec3_t(-1, 0, 0),

		vec3_t(0, -1, -1),
		vec3_t(0, 0, -1),

	};

	std::vector<vec3_t> sidesBlocksOnly = {
		vec3_t(1, 0, 0),
		vec3_t(0, 0, 1),
		vec3_t(-1, 0, 0),
		vec3_t(0, 0, -1),
		vec3_t(0, -1, 0),
	};
	bool isSideEmpty();
	bool tryButton(vec3_ti blkPlacement);
	void attackCrystal();
	void onMove(C_MoveInputHandler* hand);
	// Inherited via IModule
	virtual const char* getRawModuleName() override;
	virtual const char* getModName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable();
};