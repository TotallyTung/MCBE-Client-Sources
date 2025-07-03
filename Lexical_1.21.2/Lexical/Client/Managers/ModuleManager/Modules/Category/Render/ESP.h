#pragma once
#include "../../ModuleBase/Module.h"

class ESP : public Module {
private:
	UIColor color = UIColor(255, 255, 255);
	int alpha = 40;
	int lineAlpha = 135;
	bool mobs = false;
	bool items = false;
public:
	ESP();
	void onLevelRender() override;
};