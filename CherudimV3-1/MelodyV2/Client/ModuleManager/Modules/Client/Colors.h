#pragma once
#include "../Module.h"
#include "../../../../Utils/ColorUtils.h"

class Colors : public Module {
private:
	UIColor mainColor = UIColor(155, 40, 245, 255);
	UIColor secondaryColor = UIColor(51, 13, 81, 255);
	int modeValue = 0;
	float seconds = 3.f;
	float saturation = 1.f;
	float brightness = 1.f;
	int seperation = 100;
private:
	float openDuration = 0.f;
public:
	Colors();

	int getSeperationValue() {
		return this->seperation;
	}

	UIColor getColor(int index = 0) {
		if (modeValue == 1) return ColorUtils::getRainbowColor(seconds, saturation, brightness, index);
		if (modeValue == 2) return ColorUtils::getWaveColor(mainColor, secondaryColor, index);
		return this->mainColor;
	}

	virtual bool isEnabled() override;
	virtual void setEnabled(bool enabled) override;
	virtual bool isVisible() override;
};