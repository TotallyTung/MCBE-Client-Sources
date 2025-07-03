#include "CustomFont.h"

CustomFont::CustomFont() : IModule(0, Category::CLIENT, "Lfamo") {
	fontMode.addEntry("Minecraft", 0);
	fontMode.addEntry("ProductSans", 1);
	fontMode.addEntry("BahnSchrift", 2);
	registerEnumSetting("Font", &fontMode, 0);

	shadowMode.addEntry("None", 0);
	shadowMode.addEntry("Minecraft", 1);
	shadowMode.addEntry("Future", 2);
	registerEnumSetting("Shadow", &shadowMode, 2);
}

const char* CustomFont::getModuleName() {
	return "Font";
}

bool CustomFont::isEnabled() {
	return true;
}

bool CustomFont::isVisible() {
	return false;
}