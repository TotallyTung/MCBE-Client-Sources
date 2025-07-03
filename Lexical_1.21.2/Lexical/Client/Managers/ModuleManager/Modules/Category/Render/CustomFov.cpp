#include "CustomFov.h"

CustomFov::CustomFov() : Module("CustomFov", "Allow you modify FOV without limitation", Category::RENDER) {
	registerSetting(new SliderSetting<float>("Fov", "NULL", &fov, 120.f, 1.f, 359.f));
}