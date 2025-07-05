#include "Theme.h"

#include "../../../Utils/Utils.h"

float Theme::rFloat{1.f};
float Theme::gFloat{1.f};
float Theme::bFloat{1.f};
float Theme::rFloat4{0.f};
float Theme::gFloat4{0.f};
float Theme::bFloat4{0.f};

Theme::Theme() : IModule(0, Category::HUD, "Change the color of UI Elements.") {
	registerBoolSetting("Rainbow", &this->rainbow, rainbow);
	registerFloatSetting("Red", &rFloat, rFloat, 0.f, 1.f);
	registerFloatSetting("Green", &gFloat, gFloat, 0.f, 1.f);
	registerFloatSetting("Blue", &bFloat, bFloat, 0.f, 1.f);
	registerFloatSetting("ShadowRed", &rFloat4, rFloat4, 0.f, 1.f);
	registerFloatSetting("ShadowGreen", &gFloat4, gFloat4, 0.f, 1.f);
	registerFloatSetting("ShadowBlue", &bFloat4, bFloat4, 0.f, 1.f);
}

Theme::~Theme() {
}

const char* Theme::getModuleName() {
	return ("CustomColor");
}