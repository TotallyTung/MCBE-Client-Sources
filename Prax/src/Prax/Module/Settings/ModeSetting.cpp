#pragma once

#include "ModeSetting.h"

std::string ModeSetting::ToLower(const std::string& string)
{
	std::string str = string;
	std::ranges::transform(str, str.begin(), ::tolower);
	return str;
}

void ModeSetting::ChangeTo(std::string input) {
    int index = 0;
	for (auto mode : Modes) {
		if (ToLower(mode) == ToLower(input)) {
			SelectedIndex = index;
		}
		index++;
	}
}

void ModeSetting::HandleClick(bool wasLeft) {
	if (wasLeft) {
        if (SelectedIndex < Modes.size() - 1)
            SelectedIndex++;
        else
            SelectedIndex = 0;
    } else {
        if (SelectedIndex > 0)
            SelectedIndex--;
        else
            SelectedIndex = Modes.size() - 1;
    }

}

std::string ModeSetting::GetDisplayText() {
	return Name + ": " + Modes.at(SelectedIndex);
}

std::string ModeSetting::GetDisplayTextNoName() {
    return Modes.at(SelectedIndex);
}

int ModeSetting::GetIndexByName(std::string str) {
    int index = 0;
    // Remove spaces from the string
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

    for (auto mode : Modes) {
        // Remove spaces from the string
        std::string modeStr = mode;
        modeStr.erase(std::remove(modeStr.begin(), modeStr.end(), ' '), modeStr.end());
        if (ToLower(modeStr) == ToLower(str)) {
            return index;
        }
        index++;
    }
    return -1;
}

// return const char* const items[] for ImGuiCombo
std::string ModeSetting::GetImGuiComboText() const {
    std::string text = "";
    for (auto mode : Modes) {
        text += mode + '\0';
    }
    return text;
}