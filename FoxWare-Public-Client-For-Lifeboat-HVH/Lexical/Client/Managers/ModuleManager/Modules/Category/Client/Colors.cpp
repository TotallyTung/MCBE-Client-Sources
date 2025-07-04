#include "Colors.h"

Colors::Colors() : Module("ColorManager", "change the color of the client.", Category::CLIENT) {
    registerSetting(new EnumSetting("mode", "change colors mode", { "single", "rainbow", "wave" }, &modeValue, 0));
    registerSetting(new SliderSetting<float>("seconds", "changes rainbow speed.", &seconds, 5.f, 1.f, 10.f));
    registerSetting(new SliderSetting<float>("saturation", "changes rainbow saturation.", &saturation, 0.f, 0.f, 1.f));
    registerSetting(new SliderSetting<float>("brightness", "changes rainbow brightness.", &brightness, 0.f, 0.f, 1.f));
    registerSetting(new ColorSetting("start color", "null", &mainColor, mainColor));
    registerSetting(new ColorSetting("middle color", "null", &midColor, midColor));
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