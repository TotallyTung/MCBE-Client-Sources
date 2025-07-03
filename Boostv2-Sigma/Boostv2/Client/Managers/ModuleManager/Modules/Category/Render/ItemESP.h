#pragma once
#include "../../ModuleBase/Module.h"

class ItemESP : public Module {
private:
	UIColor color = UIColor(255, 255, 255);
	int alpha = 40;
	int lineAlpha = 135;
	bool mobs = false;
	bool items = true;
public:
	ItemESP();
	void onLevelRender() override;
};