#pragma once
#include "../../ModuleBase/Module.h"
#include "../../Utils/ColorUtil.h"
class Colors : public Module {
private:
	UIColor mainColor = UIColor(255, 178, 127, 255);
	UIColor midColor = UIColor(255, 211, 188, 255);
	int modeValue = 0;
	float seconds = 3.f;
	float saturation = 1.f;
	float brightness = 1.f;
private:
	float openDuration = 0.f;
public:
	int seperation = 100;
	Colors();
	inline int getSeperationValue() {
		return this->seperation;
	}
	inline UIColor getColor(int index = 0) {
		if (modeValue == 1) return ColorUtil::getRainbowColor(seconds, saturation, brightness, index);
		if (modeValue == 2) return ColorUtil::getWaveColor(mainColor, midColor, index);
		return this->mainColor;
	}
	virtual bool isEnabled() override;
	virtual void setEnabled(bool enabled) override;
	virtual bool isVisible() override;
};