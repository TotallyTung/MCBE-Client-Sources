#pragma once
#include "../../ModuleBase/Module.h"

class BowSpam : public Module {
private:
	int delay = 5;
public:
	BowSpam();
	void onNormalTick(LocalPlayer* localPlayer) override;
};