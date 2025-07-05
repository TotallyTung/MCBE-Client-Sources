#pragma once
#include <ctime>

#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class AutoEZ : public IModule {
private:
	bool sayUserDied = true;
	bool sayEnemDied = true;

	std::string deathMsg[4] = {
		"laglaglag",
		"TOTEMFAIL",
		"My mom disconnect the internet ",
		"1 fps moment=die"
	};

	std::string killMsg[5] = {
		"Keep barking nn",
		"Hush mode rash indian",
		"Kys nn no one loves you",
		"Owned by SLD",
		"WHY SO FUCKING TRASH?"
	};

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
	virtual void onTick(C_GameMode* gm) override;
};