#include "CustomHotbar.h"

CustomHotbar::CustomHotbar() : IModule(0, Category::CLIENT, "Hotbar, but better!") {
	registerEnumSetting("Hotbar", &hotbartype, 1);
	hotbartype.addEntry("Minecraft", 0);
	hotbartype.addEntry("Zephyr", 1);
	registerEnumSetting("Smooth", &smooth, 1);
	smooth.addEntry("None", 0);
	smooth.addEntry("Smooth", 1);
	registerFloatSetting("Speed", &speed, speed, 0, 0.2f);
}

CustomHotbar::~CustomHotbar() {
}

const char* CustomHotbar::getModuleName() {
	return ("CustomHotbar");
}