#pragma once
#include "Module.h"
class AutoTrap : public IModule {
private:

	bool tryAutoTrap(vec3_t AutoTrap);

public:
	AutoTrap();
	~AutoTrap();
	bool spoof = false;
	bool tnt = false;
	SettingEnum mode3 = this;
	int range = 7;
	bool onClick = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
