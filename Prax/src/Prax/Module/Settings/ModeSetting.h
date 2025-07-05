#pragma once
#include "Setting.h"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>


class ModeSetting : public Setting {
public:
	int SelectedIndex;
	std::vector<std::string> Modes;

    ModeSetting(std::string name, std::string description, std::vector<std::string> modes, int selectedIndex, class BoolSetting* parentSetting = nullptr) : Setting::Setting(name, description, Mode, parentSetting) {
		this->Modes = modes;
		this->SelectedIndex = selectedIndex;
	}


	std::string ToLower(const std::string& string);
	void ChangeTo(std::string input);
	void HandleClick(bool wasLeft) override;
    std::string GetDisplayText() override;
    std::string GetDisplayTextNoName();
	std::string GetImGuiComboText() const;

    int GetIndexByName(std::string str);

	operator int() const { return SelectedIndex; }
};