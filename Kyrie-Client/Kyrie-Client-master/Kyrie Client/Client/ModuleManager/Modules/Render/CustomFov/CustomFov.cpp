#include "CustomFov.h"

CustomFov::CustomFov() : Module(0, Category::RENDER, "CustomFov") {
	registerFloatSetting("Fov", &fov, fov, 10.f, 170.f);
}

const char* CustomFov::getModuleName() {
	return "CustomFov";
}