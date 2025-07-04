#pragma once

#include "../../ModuleBase/Module.h"

class AutoLeave : public Module {
public:
	int health = 20;
	AutoLeave();
	~AutoLeave();

	// Inherited via IModule
	virtual void onNormalTick(LocalPlayer* actor) override;
};