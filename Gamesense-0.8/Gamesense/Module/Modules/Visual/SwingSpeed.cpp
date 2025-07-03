#include "SwingSpeed.h"

SlowSwing::SlowSwing() : IModule(0x0, Category::VISUAL, "Slows down swing speed") {
	registerIntSetting("Speed", &swingspeed, swingspeed, 1, 30);
}

SlowSwing::~SlowSwing() {
}

const char* SlowSwing::getRawModuleName() {
	return ("SlowSwing");
}