#pragma once
#include "Module.h"
#include "../../Utils/ColorUtil.h"

class Colors : public IModule {
private:
	UIColor mainColor = UIColor(r, g, b);
	int r = 255;
	int g = 255;
	int b = 255;
	bool rainbow = false;
	float speed = 3.f;
	float saturation = 1.f;
	float brightness = 1.f;
	int seperation = 25;

public:
	Colors();
	const UIColor getColor() { return this->mainColor; }
	const MC_Color getColorV2() { return MC_Color( r,g,b); }
	virtual const char* getModuleName() override;
	virtual bool isEnabled() override;
	virtual bool isVisible() override;
	virtual void onImGuiRender(ImDrawList* d);
};