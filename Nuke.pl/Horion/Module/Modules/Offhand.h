#pragma once
#include "Module.h"

class Offhand : public IModule {
private:
	SettingEnum Mode = SettingEnum(this);

public:
	Offhand();

	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
	virtual void onTick(GameMode* gm) override;
};