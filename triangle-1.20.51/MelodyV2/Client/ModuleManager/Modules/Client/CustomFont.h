#pragma once
#include "../Module.h"
#include "../../../ConfigManager/ConfigManager.h"
class ClientManager : public Module {
private:
	UIColor mainColor = UIColor(117, 142, 236, 255);
	UIColor secondaryColor = UIColor(127, 152, 246, 255);
	ConfigManager* configMgr = nullptr;
	int modeValue = 0;
	float seconds = 3.f;
	float saturation = 1.f;
	float brightness = 1.f;
	int seperation = 100;
public:
	int fontType = 0;
	float fontsize1 = 20;
	bool fontShadow = true;
	bool fontItalic = true;
	ClientManager();
	virtual bool isEnabled() override;
	virtual void setEnabled(bool enabled) override;
	virtual bool isVisible() override;
	virtual void onNormalTick(Actor* actor) override;
	int getSeperationValue() {
		return this->seperation;
	}

	UIColor getColor(int index = 0) {
		if (modeValue == 1) return ColorUtils::getRainbowColor(seconds, saturation, brightness, index);
		if (modeValue == 2) return ColorUtils::getWaveColor(mainColor, secondaryColor, index);
		return this->mainColor;
	}
};