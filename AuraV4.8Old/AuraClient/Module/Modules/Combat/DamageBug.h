#pragma once
//#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class DamageBug : public IModule {
public:
	bool targetListEmptyd = false;
	int slotdb = 0;
	float range = 7;
	int dl11 = 0;

	std::string name = "DamageBug";

	DamageBug();

	virtual const char* getRawModuleName();
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm);
	//virtual void onPlayerTick(C_Player* plr);

};