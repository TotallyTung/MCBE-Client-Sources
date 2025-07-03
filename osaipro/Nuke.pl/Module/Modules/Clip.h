#pragma once

#include "Module.h"
class Clip : public IModule {
public:
	Clip();
	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onKeyUpdate(int key, bool isDown) override;
	virtual void onDisable() override;
};