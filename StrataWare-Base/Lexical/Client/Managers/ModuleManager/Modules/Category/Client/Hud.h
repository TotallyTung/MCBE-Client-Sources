#pragma once
#include "../../ModuleBase/Module.h"

class Hud : public Module {
private:
public:
	bool arraylist = false;
	float arraylisty = 0;
	float arraylistx = 0;
	bool watermark = false;
	float watermarky = 0;
	float watermarkx = 0;
	bool bold = false;
	bool showPosition;
	int positionOffsetX;
	int positionOffsetY;
	bool showHealth;
	int healthOffsetX;
	int healthOffsetY;
	Hud();
	bool isEnabled() override;
	void onD2DRender() override;
};
