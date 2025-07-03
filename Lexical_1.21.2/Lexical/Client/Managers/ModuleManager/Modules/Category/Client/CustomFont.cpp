#include "CustomFont.h"

CustomFont::CustomFont() : Module("Fonts", "Font of Client", Category::CLIENT) {
	fontEnumSetting = (EnumSetting*)registerSetting(new EnumSetting("Font", "NULL", { "Arial", "Consolas", "Bahnschrift", "Product Sans"}, &fontMode, 0));
	registerSetting(new SliderSetting<int>("FontSize", "NULL", &fontSize, 25, 20, 30));
	registerSetting(new BoolSetting("Italic", "NULL", &italic, false));
	registerSetting(new BoolSetting("Shadow", "NULL", &shadow, true));
}

bool CustomFont::isEnabled() {
	return true;
}

bool CustomFont::isVisible() {
	return false;
}

std::string CustomFont::getSelectedFont() {
	return fontEnumSetting->enumList[fontMode];
}