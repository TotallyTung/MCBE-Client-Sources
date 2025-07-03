#include "Colors.h"
Colors::Colors() : IModule(0, Category::CLIENT, "Colors") {
	registerFloatSetting("Saturation", &saturation, saturation, 0.f, 1.f);
	registerIntSetting("Spacing", &spacing, spacing, 1, 200);

	registerFloatSetting("Red", &r, r, 0.f, 255.f);
	registerFloatSetting("Green", &g, g, 0.f, 255.f);
	registerFloatSetting("Blue", &b, b, 0.f, 255.f);
	registerFloatSetting("Red2", &r2, r2, 0.f, 255.f);
	registerFloatSetting("Green2", &g2, g2, 0.f, 255.f);
	registerFloatSetting("Blue2", &b2, b2, 0.f, 255.f);
	registerEnumSetting("Color", &color, 0);
	color.addEntry("Rainbow", 0);
	color.addEntry("Astolfo", 1);
	color.addEntry("Wave", 2);
	color.addEntry("RGBWave", 3); 
	registerFloatSetting("Speed", &speed, speed, 0.5f, 5.f);
	registerFloatSetting("Brightness", &brightness, brightness, 0.f, 1.f);
}

const char* Colors::getModuleName() {
	return "Colors";
}

bool Colors::isEnabled() {
	return true;
}

bool Colors::isVisible() {
	return false;
}
void Colors::onImGuiRender(ImDrawList* d) {
	static AntiCrystal* ac = moduleMgr->getModule<AntiCrystal>();
	Vec2 windowSize = Game.getGuiData()->windowSizeReal;
	static const float textSize = 2.f;
	static const float textPadding = 1.f * textSize;
	static const float textHeight = ImGuiUtils::getTextHeight(textSize);
	float y = windowSize.y - textHeight - textPadding;
	if (!ac->isEnabled() && ac->Warning) {
		std::string showtxt = "AntiCrystal is Not Enabled";
		ImGuiUtils::drawText(Vec2(windowSize.x / 2.8, y / 2), showtxt.c_str(), UIColor(255, 0, 0, 255), textSize);
	}
}