#pragma once
#include "../../../Utils/Utils.h"
#include "../../../Utils/Logger.h"
#include "../ModuleManager.h"
#include "Module.h"

class NoEntityTrace : public IModule
{
private:
	bool noWeapon = true;
	bool pickOnly = true;
	bool gapple = false;

public:
	NoEntityTrace();
	~NoEntityTrace();

	bool allMobs = true; // keep this true

	// Inherited via IModule
	virtual const char *getModuleName() override;
	virtual void onTick(GameMode *gm) override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	virtual void onJoiningServer() override;
};
