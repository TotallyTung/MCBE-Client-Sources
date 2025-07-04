#pragma once
#include "../../ModuleBase/Module.h"

class AirJump : public Module {
public:
	AirJump();
	void onLevelTick(Level* level) override;
};