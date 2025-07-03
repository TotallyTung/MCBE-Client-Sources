#pragma once
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class TNTAura : public IModule {
private:
	int placedelay = 1;
	int ignitedelay = 3;
	Vec3 neckBreaker;
public:
	bool Self = false;
	std::vector<Vec3> obs;
	SettingEnum ObsidianMode;
	int Normal = 0;
	int AntiThroughWall = 1;
	int OnlyPillar = 2;
	int AntiCollapse = 3;
	int OnlyGravity = 4;
	int keys = false;
	bool render = true;
	TNTAura();
	~TNTAura();
	int range = 5;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onKeyUpdate(int keyb, bool isDown) override;
	static void onJoiningServer();
	static bool findEntByC(Entity* curEnt, bool isRegularEntity);
};