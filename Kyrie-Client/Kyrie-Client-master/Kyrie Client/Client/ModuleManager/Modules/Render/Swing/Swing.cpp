#include "Swing.h"

Swing::Swing() : Module(0x0, Category::RENDER, "Swing") {
	registerIntSetting("Speed", &speed, speed, 1, 20);
}

const char* Swing::getModuleName() {
	return "Swing";
}