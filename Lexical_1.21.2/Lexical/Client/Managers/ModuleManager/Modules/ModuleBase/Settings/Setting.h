#pragma once
#include <string>

enum class SettingType {
	UNKNOW_S,
	BOOL_S,
	KEYBIND_S,
	ENUM_S,
	COLOR_S,
	SLIDER_S, 
	PAGE_S
};

class Setting {
public:
	std::string name;
	std::string description;
	int settingPage = 0;
	SettingType type = SettingType::UNKNOW_S;
public:
	// ClickGUI stuff
	float selectedAnim = 0.f;
};
