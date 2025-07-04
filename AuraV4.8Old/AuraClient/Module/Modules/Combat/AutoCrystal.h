#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class AutoCrystal : public IModule {
private:
	bool gapplehb = false;
	int gappleSlot = 4;

	bool obsidianhb = false;
	int obsidianSlot = 5;

	bool crystalhb = false;
	int crystalSlot = 6;

	bool strengthhb = false;
	int strengthSlot = 7;

	bool dagapple = false;
	bool daob = false;
	bool dacry = false;
	bool dastrength = false;
	
	bool Nodelay = false;
public:

	std::string name = "AutoHotbar";
	//SettingEnum mode = this;

	virtual const char* getRawModuleName() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	AutoCrystal();
};
