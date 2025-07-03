#pragma once
#include "../../ModuleBase/Module.h"

class BreakIndicators : public Module {
private:
	UIColor startColor = UIColor(255, 0, 0);
	UIColor endColor = UIColor(0, 255, 0);
	int alpha = 40;
	int lineAlpha = 175;
public:
	void onLevelRender() override;
	BreakIndicators();
};