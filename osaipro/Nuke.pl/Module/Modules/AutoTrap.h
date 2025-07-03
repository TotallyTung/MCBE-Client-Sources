#pragma once
#include "Module.h"
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
class AutoTrap : public IModule {
private:

	bool tryAutoTrap(Vec3 AutoTrap);

public:
	AutoTrap();
	~AutoTrap();
	SettingEnum mode = this;
	int range = 7;
	bool onClick = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onJoiningServer() override;
};
