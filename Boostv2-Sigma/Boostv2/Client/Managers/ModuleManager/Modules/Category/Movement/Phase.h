#pragma once
#include "../../ModuleBase/Module.h"

class Phase : public Module {
public:
	Phase();

	void onDisable() override;
	void onLevelTick(Level* level) override;
};
