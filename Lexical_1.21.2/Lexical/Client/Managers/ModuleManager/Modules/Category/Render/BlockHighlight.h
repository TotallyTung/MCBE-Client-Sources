#pragma once
#include "../../ModuleBase/Module.h"

class BlockHighlight : public Module {
private:
	UIColor color = UIColor(255, 255, 255);
	int alpha = 40;
	int lineAlpha = 175;
	bool slide = false;
	float slideSpeed = 5.f;
	AABB oldRenderAABB;
public:
	BlockHighlight();

	void onLevelRender() override;
};