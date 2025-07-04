#include "FontChanger.h"
#include "../../Utils/Utils.h"

float Theme::rFloat{ 0.f };
float Theme::gFloat{ 0.f };
float Theme::bFloat{ 1.f };

Theme::Theme() : IModule(0, Category::OTHER, "Change the color of UI Elements.") {

	registerFloatSetting("Red", &rFloat, rFloat, 0.f, 1.f);
	registerFloatSetting("Green", &gFloat, gFloat, 0.f, 1.f);
	registerFloatSetting("Blue", &bFloat, bFloat, 0.f, 1.f);
	registerBoolSetting("Rainbow", &this->rainbow, this->rainbow);
	registerBoolSetting("Outline", &this->outline, this->outline);
}

Theme::~Theme() {
}

const char* Theme::getModuleName() {
	return ("XCX-Theme");
}