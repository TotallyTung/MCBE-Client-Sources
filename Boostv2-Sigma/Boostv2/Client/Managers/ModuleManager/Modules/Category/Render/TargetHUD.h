#pragma once
#include "../../ModuleBase/Module.h"

class TargetHUD : public Module {
private:
	bool useRounded = false; 
	bool smoothHurt = false;
	bool useOutline = false;
	int hudPosition = 0; // 0 = WorldToScreen, 1 = Hotbar
public:
	TargetHUD();
	void onD2DRender() override;
};