#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"

class AutoClip : public IModule {
public:
	AutoClip();
	~AutoClip();
	bool abc=true;
	const char* name = "AutoClip";
	SettingEnum mode = this;
	virtual const char* getModName() override;
	virtual const char* getRawModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	virtual void onMove(C_MoveInputHandler* hand) override;
};


