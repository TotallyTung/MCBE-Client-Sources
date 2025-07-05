#pragma once
#include "Module.h"

class Spammer : public IModule {
private:
	int delay = 1;
	int length = 5;
	int messageTimer = 0;
	int messageIndex = 0;

public:
	bool bypass = false;
	const char* LoholismList[14] = {"Team Aura Owns All", "AuraV4 best client", "So ez for Aura", "Aura Owns YOU", "Why use other clients when Aura V4 exists", "AuraV4 is too good", "@here https://discord.gg/DpD9RqAc4q", "too ez for Aura", "Sit down weaks Aura owns all", "Aura > ALL", "Strongest client for spvp: AuraV4", "Why use any other client?", "AuraV4 Owns IGN", "Join Team Aura Today!"};

	inline int& getDelay() { return delay; };
	SettingEnum messageMode;

	Spammer();
	~Spammer();

	const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
};
