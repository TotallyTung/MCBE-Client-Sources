#pragma once
#include "../../ModuleBase/Module.h"

class AutoJump : public Module {
public:
	AutoJump();
	void onLevelTick(Level* level) override;
	void onDisable() override;
};