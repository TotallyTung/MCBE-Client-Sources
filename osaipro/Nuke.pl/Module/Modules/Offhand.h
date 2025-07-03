#pragma once
#include "Module.h"

class AutoTotem : public IModule {
public:
	std::string name = ("Offhand");
	SettingEnum item;
	SettingEnum activation = this;
	SettingEnum cc = this;
	bool notify = false;
	bool setOffhand = false;
	SettingEnum mode = this;
	void runNukkit();
	void offhandNotify();
	AutoTotem();

	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
	virtual void onTick(GameMode* gm) override;
};
