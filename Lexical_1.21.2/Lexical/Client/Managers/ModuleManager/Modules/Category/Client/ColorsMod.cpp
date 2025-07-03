#include "ColorsMod.h"

Colors::Colors() : Module("Colors", "change the color of the client.", Category::CLIENT) {
    registerSetting(new EnumSetting("Mode", "change colors mode", { "Single", "RGB", "Wave" }, &modeValue, 1));
    registerSetting(new SliderSetting<float>("Seconds", "changes rainbow speed.", &seconds, 5.f, 1.f, 10.f));
    registerSetting(new SliderSetting<float>("Saturation", "changes rainbow saturation.", &saturation, 1.f, 0.f, 1.f));
    registerSetting(new SliderSetting<float>("Brightness", "changes rainbow brightness.", &brightness, 1.f, 0.f, 1.f));
    registerSetting(new SliderSetting<int>("Seperation", "changes seperation.", &seperation, 125, 0, 255));
    registerSetting(new ColorSetting("Start color", "null", &mainColor, UIColor(125, 140, 255), true));
    registerSetting(new ColorSetting("End color", "null", &midColor, UIColor(100, 115, 235)));
}

bool Colors::isEnabled() {
    return true;
}

void Colors::setEnabled(bool enabled) {
}

bool Colors::isVisible() {
    return false;
}