#pragma once
#include "../../ModuleBase/Module.h"

class Colors : public Module {
private:
	UIColor mainColor = UIColor(117, 142, 236, 255);
	UIColor secondaryColor = UIColor(127, 152, 246, 255);
	enum Mode {
		STATIC = 0,
		RAINBOW = 1,
		WAVE = 2
	};
	float seconds = 3.f;
	float saturation = 1.f;
	float brightness = 1.f;
	int seperation = 100;
	int modeValue = STATIC; 
private:
	float openDuration = 0.f;
public:
	Colors();

	int getSeperationValue() {
		return this->seperation;
	}

	UIColor getColor(int index = 0) {
		if (modeValue == RAINBOW) return ColorUtil::getRainbowColor(seconds, saturation, brightness, index);
		if (modeValue == WAVE) return ColorUtil::getWaveColor(mainColor, secondaryColor, index);
		return this->mainColor;
	}

	virtual bool isEnabled() override;
	virtual void setEnabled(bool enabled) override;
	virtual bool isVisible() override;
};