#include "Velocity.h"

Velocity::Velocity() : Module("Velocity", "Prevent you from knockback", Category::MOVEMENT) {
	registerSetting(new SliderSetting<float>("Horizontal", "NULL", &horizontal, horizontal, 0.f, 100.f));
	registerSetting(new SliderSetting<float>("Vertical", "NULL", &vertical, vertical, 0.f, 100.f));
}

std::string Velocity::getModeText() {
	static char textStr[15];
	sprintf_s(textStr, 15, "H%i%%V%i%%", (int)(std::round(horizontal)), (int)(std::round(vertical)));
	return std::string(textStr);
}