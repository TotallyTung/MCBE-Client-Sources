#include "CustomFont.h"

CustomFont::CustomFont() : Module("Fonts", "Font of Client", Category::CLIENT) {
    fontEnumSetting = (EnumSetting*)registerSetting(new EnumSetting("Font", "NULL", {
        "Arial", "Bahnschrift", "Calibri", "Cambria", "Candara", "Product Sans", "Verdana",
        "Berlin Sans FB", "Blackadder ITC", "Magneto", "Arial Rounded", "Ebrima", "Corbel", "Century Gothic", "Console"
        }, &fontMode, 14));

    registerSetting(new NumberSetting<int>("FontSize", "NULL", &fontSize, 25, 10, 30));
    registerSetting(new ToggleSetting("Italic", "NULL", &italic, false));
    registerSetting(new ToggleSetting("Shadow", "NULL", &shadow, false));
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