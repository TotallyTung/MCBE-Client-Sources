#pragma once

#include "Module.h"

class AutoPhase : public IModule {
public:
	AutoPhase();
	~AutoPhase();

	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onTick(GameMode* gm) override;
};
