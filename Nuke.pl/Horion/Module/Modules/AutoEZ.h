#pragma once
#include <ctime>
#include <stdlib.h> /* srand, rand */
#include "../../../Utils/Utils.h"
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class AutoEZ : public IModule {
private:
	bool sayUserDied = true;
	bool sayEnemDied = true;
	SettingEnum MsgType = this;
public:
	int kc = 0;
	int dc = 0;
	bool foundPlayer = false;
	bool isDead = false;
	bool antiSpamCheck = true;

	AutoEZ();
	~AutoEZ();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};