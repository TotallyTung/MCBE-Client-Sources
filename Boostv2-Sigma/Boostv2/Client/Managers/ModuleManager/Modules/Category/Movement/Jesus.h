#pragma once

#include "../../ModuleBase/Module.h"

class Jesus :
	public Module
{
private:
	bool wasInWater = false;
public:
	Jesus();
	~Jesus();

	// Inherited via IModule
	virtual void onNormalTick(LocalPlayer* gm) override;
};

