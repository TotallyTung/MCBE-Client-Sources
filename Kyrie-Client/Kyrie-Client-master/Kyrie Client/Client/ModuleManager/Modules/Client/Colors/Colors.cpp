#include "Colors.h"

Colors::Colors() : Module(0x0, Category::CLIENT, "Colors") {
	Mode.addEntry("Single", 0);
	Mode.addEntry("Rainbow", 1);
	Mode.addEntry("Wave", 2);
	registerEnumSetting("Mode", &Mode, 0, nullptr);

	Check1 = (Mode.getSelectedValue() == 0 || Mode.getSelectedValue() == 2);
	Check2 = Mode.getSelectedValue() == 1;
	Check3 = Mode.getSelectedValue() == 2;

	registerIntSetting("Red", &r, r, 0, 255, nullptr, &Check1);
	registerIntSetting("Green", &g, g, 0, 255, nullptr, &Check1);
	registerIntSetting("Blue", &b, b, 0, 255, nullptr, &Check1);

	registerIntSetting("Red2", &r2, r2, 0, 255, nullptr, &Check3);
	registerIntSetting("Green2", &g2, g2, 0, 255, nullptr, &Check3);
	registerIntSetting("Blue2", &b2, b2, 0, 255, nullptr, &Check3);

	registerFloatSetting("Speed", &speed, speed, 1.f, 5.f, nullptr, &Check2);
	registerFloatSetting("Saturation", &saturation, saturation, 0.f, 1.f, nullptr, &Check2);
	registerFloatSetting("Brightness", &brightness, brightness, 0.f, 1.f, nullptr, &Check2);
}

Colors::~Colors() {
}

const char* Colors::getModuleName() {
	return "Colors";
}

bool Colors::isEnabled() {
	return true;
}

void Colors::onImGuiRender(ImDrawList* d) {
}

void Colors::onModifySettingEnum(SettingEnum* se) {
	if (se == &Mode) {
		Check1 = (Mode.getSelectedValue() == 0 || Mode.getSelectedValue() == 2);
		Check2 = Mode.getSelectedValue() == 1;
		Check3 = Mode.getSelectedValue() == 2;
	}
}