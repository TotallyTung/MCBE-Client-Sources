#include "Colors.h"
#include "../ModuleManager.h"
Colors::Colors() : IModule(0, Category::CLIENT, "Colors") {
	registerIntSetting("Red", &r, r, 0, 255);
	registerIntSetting("Green", &g, g, 0, 255);
	registerIntSetting("Blue", &b, b, 0, 255);
	registerBoolSetting("Rainbow", &rainbow, rainbow);
	registerFloatSetting("Speed", &speed, speed, 0.5f, 5.f);
	registerFloatSetting("Brightness", &brightness, brightness, 0.f, 1.f);
	registerIntSetting("Seperation", &seperation, seperation, 0, 100);
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
	if (rainbow) {
		this->mainColor = ColorUtil::getRainbowColor(speed, saturation, brightness, 0);
	}
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