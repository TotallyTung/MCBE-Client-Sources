#pragma once
#include "../../ModuleBase/Module.h"

class Reach : public Module {
private:

	float* SurvivalreachPtr = nullptr;
public:
	float reachValue = 4.f;
	Reach();

	virtual void onEnable() override;
	virtual void onNormalTick(LocalPlayer* localPlayer) override;
};
