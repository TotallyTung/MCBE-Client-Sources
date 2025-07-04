#pragma once
#include "Setting.h"

class KeybindSetting : public Setting {
public:
	int* value;

	KeybindSetting(std::string settingName, std::string des, int* ptr, int defaultValue) {
		this->name = settingName;
		this->description = des;
		this->value = ptr;
		*this->value = defaultValue;
		this->type = SettingType::KEYBIND_S;
	}
};