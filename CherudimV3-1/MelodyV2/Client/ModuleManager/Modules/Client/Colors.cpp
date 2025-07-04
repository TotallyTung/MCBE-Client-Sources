#include "Colors.h"

Colors::Colors() : Module("Colors", "Change the color of UI Elements.", Category::CLIENT) {
	addEnumSetting("Mode", "Change colors mode", {"Single", "Rainbow", "Wave"}, &modeValue);
	addSlider<float>("Seconds", "Changes rainbow speed.", ValueType::FLOAT_T, &seconds, 1.f, 10.f);
	addSlider<float>("Saturation", "Changes rainbow saturation.", ValueType::FLOAT_T, &saturation, 0.f, 1.f);
	addSlider<float>("Brightness", "Changes rainbow brightness.", ValueType::FLOAT_T, &brightness, 0.f, 1.f);
	addSlider<int>("Seperation", "Changes color seperation.", ValueType::INT_T, &seperation, 0, 250);
	addColorPicker("Color", "NULL", &mainColor);
	addColorPicker("SColor", "NULL", &secondaryColor);
}

bool Colors::isEnabled() {
	return true;
}

void Colors::setEnabled(bool enabled) {
	// Do nothing :D
}

bool Colors::isVisible() {
	return false;
}