#include "CustomFont.h"

CustomFont::CustomFont() : Module("Fonts", "Font of Client", Category::CLIENT) {
	fontEnumSetting = (EnumSetting*)registerSetting(new EnumSetting("Font", "NULL", { "Arial", "Bahnschrift", "Verdana", "Impact", "Lucida Console", "Trebuchet MS", "Idk" }, &fontMode, 5));
	registerSetting(new SliderSetting<int>("FontSize", "NULL", &fontSize, 19, 10, 30));
	registerSetting(new BoolSetting("Italic", "NULL", &italic, true));
	registerSetting(new BoolSetting("Shadow", "NULL", &shadow, shadow));
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