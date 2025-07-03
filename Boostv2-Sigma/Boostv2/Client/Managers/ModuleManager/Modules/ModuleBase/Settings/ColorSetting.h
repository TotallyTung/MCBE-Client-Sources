#pragma once
#include <vector>
#include "Setting.h"
#include "SliderSetting.h"
#include "../../../../../../Utils/ColorUtil.h"

class ColorSetting : public Setting {
public:
	UIColor* colorPtr;
	std::vector<SliderSetting<uint8_t>*> colorSliders;

	bool extended = false;
	Vec2<float> pos = { 1.f, 1.f };     
	float hueDuration = 0.f;            
	bool isDraggingHue = false;
	bool isDraggingSB = false;

	ColorSetting(std::string settingName, std::string des, UIColor* ptr, UIColor defaultValue, bool alpha = true, int page = 0) {
		this->name = settingName;
		this->description = des;
		this->colorPtr = ptr;
		this->settingPage = page;
		colorSliders.push_back(new SliderSetting<uint8_t>("Red", "NULL", &colorPtr->r, defaultValue.r, 0, 255));
		colorSliders.push_back(new SliderSetting<uint8_t>("Green", "NULL", &colorPtr->g, defaultValue.g, 0, 255));
		colorSliders.push_back(new SliderSetting<uint8_t>("Blue", "NULL", &colorPtr->b, defaultValue.b, 0, 255));

		if (alpha)
			colorSliders.push_back(new SliderSetting<uint8_t>("Alpha", "NULL", &colorPtr->a, defaultValue.a, 0, 255));

		this->type = SettingType::COLOR_S;
	}

	~ColorSetting() {
		for (auto& slider : colorSliders) {
			delete slider;
			slider = nullptr;
		}
		colorSliders.clear();
	}
};
