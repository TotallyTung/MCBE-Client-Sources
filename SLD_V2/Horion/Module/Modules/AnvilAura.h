#pragma once
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class AnvilAura : public IModule {
private:
	bool hBreak = false;
	int tPlaced = 1;
	bool airplace = true;
	int maxSwitchDelay = 8;

	std::vector<vec3_t> sidesBlocksOnly = {
		vec3_t(1, 0, 0),
		vec3_t(0, 0, 1),
		vec3_t(-1, 0, 0),
		vec3_t(0, 0, -1),
	};

	int switchDelay = 0;
	bool anvilInHand = false;
	bool placedAnvil = false;
	bool pickInHand = false;

public:
	int origSlot;

	int AArange = 7;
	AnvilAura();
	~AnvilAura();


	bool isSidesAllAir(vec3_t locFromPos);

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};