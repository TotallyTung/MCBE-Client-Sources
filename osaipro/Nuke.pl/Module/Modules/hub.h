#pragma once

#include "Module.h"

class hub : public IModule {
private:
public:
	hub();
	~hub();
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
