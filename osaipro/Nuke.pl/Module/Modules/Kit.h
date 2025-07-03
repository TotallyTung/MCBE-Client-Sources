#pragma once

#include "Module.h"

class Kit : public IModule {
private:
public:
	Kit();
	~Kit();
	SettingEnum dim = this;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
