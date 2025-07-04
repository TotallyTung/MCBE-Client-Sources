#include "CustomFont.h"

CustomFont::CustomFont() : Module("CustomFont", "Changes your current Client font.", Category::CLIENT) {
	addEnumSetting("Font", "NULL", {
		"Mojangles",
		"ProductSans",
		"ProductSansLight",
		"NotoSans",
		}, 
		&fontType
	);
	addBoolCheck("Shadow", "NULL", &fontShadow);
}

bool CustomFont::isEnabled() {
	return true;
}

void CustomFont::setEnabled(bool enabled) {
	// Do nothing :D
}

bool CustomFont::isVisible() {
	return false;
}