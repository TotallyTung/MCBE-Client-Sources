#pragma once
#include "../../ModuleBase/Module.h"

class Step : public Module {
private:
	int value = 2;
public:
	Step();

	void onNormalTick(LocalPlayer* localPlayer) override;
	void onDisable() override;
};
