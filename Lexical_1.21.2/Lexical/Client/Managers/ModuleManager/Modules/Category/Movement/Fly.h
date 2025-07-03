#pragma once
#include "../../ModuleBase/Module.h"

class Fly : public Module {
private:
	float hSpeed = 1.f;
	float currentSpeed = 1.f;
	float vSpeed = 0.5f;
	float Glide = -0.02f;

public:
	Fly();
	void onNormalTick(LocalPlayer* localPlayer) override;
};