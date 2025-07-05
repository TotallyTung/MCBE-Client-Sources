#pragma once
#include "Module.h"
class AutoLog : public IModule {
public:
	int health = 3;
	int timer;
	AutoLog();
	~AutoLog();
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
