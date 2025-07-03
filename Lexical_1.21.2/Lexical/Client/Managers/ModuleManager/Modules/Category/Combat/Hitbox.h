#pragma once
#include "../../ModuleBase/Module.h"

class Hitbox : public Module {
private:
	float height = 2.f;
	float width = 4.f;
	bool includeMobs = false;
public:
	Hitbox();

	void onNormalTick(LocalPlayer* localPlayer);
};