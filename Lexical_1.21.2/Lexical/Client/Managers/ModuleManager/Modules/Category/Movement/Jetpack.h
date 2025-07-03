#pragma once
#include "../../ModuleBase/Module.h"

class Jetpack : public Module {
private:
	float speed = 1.f;
public:
	Jetpack();
	void onNormalTick(LocalPlayer* localPlayer) override;
};
