#include "CustomFont.h"

CustomFont::CustomFont() : Module(0x0, Category::CLIENT, "CustomFont") {
	this->Fonts.addEntry("Arial", 0);
	this->Fonts.addEntry("Mojangles", 1);
	registerEnumSetting("Font", &Fonts, 0);
	registerBoolSetting("Shadow", &textShadow, textShadow);
}

const char* CustomFont::getModuleName() {
	return "CustomFont";
}

bool CustomFont::isEnabled() {
	return true;
}

void CustomFont::onImGuiRender(ImDrawList* d) {
	ImGuiIO& io = ImGui::GetIO();
	ImFont* arialFont = io.Fonts->Fonts[0];
	ImFont* mojanglesFont = io.Fonts->Fonts[1];
	if (Fonts.getSelectedValue() == 0) {
		if (io.FontDefault != arialFont) io.FontDefault = arialFont;
	}
	else {
		if (io.FontDefault != mojanglesFont) io.FontDefault = mojanglesFont;
	}
}

void CustomFont::onModifySettingEnum(SettingEnum* se) {
}