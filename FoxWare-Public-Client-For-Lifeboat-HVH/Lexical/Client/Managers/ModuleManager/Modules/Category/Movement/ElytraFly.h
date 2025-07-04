#pragma once
#include "../../ModuleBase/Module.h"

class ElytraFly : public Module {
public:
	float Speed = 4.f;
	int upKey = VK_TAB;
	int downKey = VK_SHIFT;
	ElytraFly();

	virtual void onNormalTick(LocalPlayer* actor) override;
};
