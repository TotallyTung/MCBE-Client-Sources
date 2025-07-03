#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class PointBreak : public IModule {
public:
	int range = 5;
	int keybinds = false;
	SettingEnum mode;
	bool breaking = false;
	PointBreak();
	~PointBreak();
	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onKeyUpdate(int keyb, bool isDown) override;
	virtual void onJoiningServer() override;
};
