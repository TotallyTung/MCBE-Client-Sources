#pragma once

#include "Module.h"

class AutoDupe : public IModule {
private:
public:
	AutoDupe();
	~AutoDupe();
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
