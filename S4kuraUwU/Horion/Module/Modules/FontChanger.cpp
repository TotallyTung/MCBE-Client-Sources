#include "FontChanger.h"
FontChanger::FontChanger() : IModule(0, Category::HUD, "Font Changer") {
	//registerEnumSetting("Fonts", &Fonts, 0);
	registerIntSetting("Font", &this->font, this->font, 0, 9);
	/* Fonts = SettingEnum(this)
				.addEntry(EnumEntry("Notosans", 0))
				.addEntry(EnumEntry("Mojangles", 1));*/
}
FontChanger::~FontChanger() {
}
const char* FontChanger::getModuleName() {
	return ("CustomFont");
}
