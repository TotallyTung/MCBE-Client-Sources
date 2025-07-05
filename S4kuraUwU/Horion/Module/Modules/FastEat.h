#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class FastEat : public IModule {
public:
	FastEat();
	~FastEat();
	int duration = 16;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual const char* getRawModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};