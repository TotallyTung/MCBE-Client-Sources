#pragma once

#include "../../ModuleBase/Module.h"

class AutoEZ : public Module {
public:
	AutoEZ();
	~AutoEZ();

	// Inherited via IModule
	virtual void onNormalTick(LocalPlayer* actor) override;
};