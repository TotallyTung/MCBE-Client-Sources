#include "Colors.h"

Colors::Colors() : Module("Colors", "Change the color of UI Elements.", Category::CLIENT) {
	registerSetting(new EnumSetting("Mode", "Change colors mode", { "Single", "Rainbow", "Wave" }, &modeValue, 1));
	registerSetting(new SliderSetting<float>("Seconds", "Changes rainbow speed.", &seconds, 10.f, 1.f, 10.f));
	registerSetting(new SliderSetting<float>("Saturation", "Changes rainbow saturation.", &saturation, 1.f, 0.f, 1.f));
	registerSetting(new SliderSetting<float>("Brightness", "Changes rainbow brightness.", &brightness, 1.f, 0.f, 1.f));
	registerSetting(new SliderSetting<int>("Seperation", "Changes color seperation.", &seperation, 125, 0, 250));
	registerSetting(new ColorSetting("Color", "NULL", &mainColor, { 0, 0, 255 }));
	registerSetting(new ColorSetting("SColor", "NULL", &secondaryColor, { 255, 0, 0 }));
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