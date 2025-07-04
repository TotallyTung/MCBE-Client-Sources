#pragma once
#include "../../Module.h"

class Colors : public Module {
private:
	int r = 45;
	int g = 25;
	int b = 255;
	int r2 = 255;
	int g2 = 255;
	int b2 = 255;
	float speed = 3.f;
	float saturation = 1.f;
	float brightness = 1.f;
	SettingEnum Mode = SettingEnum(this);
	bool Check1 = false;
	bool Check2 = false;
	bool Check3 = false;
public:

	Colors();
	~Colors();
	Color getColor() {
		if (Check2) return ColorUtils::getRainbowColor(speed, saturation, brightness, 1);
		if (Check3) return ColorUtils::getWaveColor(r, g, b, r2, g2, b2, 1);
		return Color(r, g, b);
	}
	virtual const char* getModuleName() override;
	virtual bool isEnabled() override;
	virtual void onImGuiRender(ImDrawList* d) override;
	virtual void onModifySettingEnum(SettingEnum* se) override;
};