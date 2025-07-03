#pragma once
#include "../../ModuleBase/Module.h"

class Spider : public Module {
private:
	float speed = 0.5f;
	bool wasHorizontalCollision = false;
public:
	Spider();
	void onNormalTick(LocalPlayer* localPlayer) override;
};