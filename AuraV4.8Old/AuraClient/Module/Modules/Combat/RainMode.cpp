#include "RainMode.h"

RainMode::RainMode() : IModule(0, Category::COMBAT, "Activating this module allows you to switch to trident while using AutoWeapon in KillauraNA.") {
}
RainMode::~RainMode() {
}

const char* RainMode::getModuleName() {
	return "RainSwitch";
}