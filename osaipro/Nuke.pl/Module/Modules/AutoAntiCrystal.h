#pragma once

#include "Module.h"

class AutoAntiCrystal : public IModule {
private:

public:
	float range = 5.f;
	int counter = 0;
	int enable = 0;
	AutoAntiCrystal();
	~AutoAntiCrystal();
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onJoiningServer() override;
};
