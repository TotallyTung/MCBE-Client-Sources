#pragma once
#include "../../ModuleBase/Module.h"

class AirPlace : public Module {
private:
	bool render = true;
	UIColor color = UIColor(0, 255, 0);
	int opacity = 0;
	int lineOpacity = 255;

	bool shouldPlace = true;
	AABB renderAABB;
	bool shouldRender = false;
public:
	AirPlace();

	void onNormalTick(LocalPlayer* localPlayer) override;
	void onLevelRender() override;
};