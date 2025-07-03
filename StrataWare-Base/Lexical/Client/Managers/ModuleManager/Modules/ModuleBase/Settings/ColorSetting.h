#pragma once
#include <vector>
#include "Setting.h"
#include "NumberSetting.h"
#include "../../../../../../Utils/ColorUtil.h"

class ColorSetting : public Setting {
public:
	UIColor* colorPtr;
	std::vector<NumberSetting<uint8_t>*> colorSliders;

	bool extended = false;

	ColorSetting(std::string settingName, std::string des, UIColor* ptr, UIColor defaultValue, bool alpha = true) {
		this->name = settingName;
		this->description = des;
		this->colorPtr = ptr;

		colorSliders.push_back(new NumberSetting<uint8_t>("Red", "NULL", &colorPtr->r, defaultValue.r, 0, 255));
		colorSliders.push_back(new NumberSetting<uint8_t>("Green", "NULL", &colorPtr->g, defaultValue.g, 0, 255));
		colorSliders.push_back(new NumberSetting<uint8_t>("Blue", "NULL", &colorPtr->b, defaultValue.b, 0, 255));

		if (alpha)
			colorSliders.push_back(new NumberSetting<uint8_t>("Alpha", "NULL", &colorPtr->a, defaultValue.a, 0, 255));

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
