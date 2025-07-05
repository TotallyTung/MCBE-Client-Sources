#pragma once
#include <ctime>

#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class AutoEZ : public IModule {
private:
	bool sayUserDied = true;
	bool sayEnemDied = true;
	bool sayEnemDiedGG = true;

public:
	bool foundPlayer = false;
	bool isDead = false;
	bool antiSpamCheck = false;
	AutoEZ();
	~AutoEZ();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};