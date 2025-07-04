#pragma once
#include "Setting.h"

class BoolSetting : public Setting {
public:
	bool* value;

	BoolSetting(std::string settingName, std::string des, bool* ptr, bool defaultValue) {
		this->name = settingName;
		this->description = des;
		this->value = ptr;
		*this->value = defaultValue;
		this->type = SettingType::BOOL_S;
	}
};