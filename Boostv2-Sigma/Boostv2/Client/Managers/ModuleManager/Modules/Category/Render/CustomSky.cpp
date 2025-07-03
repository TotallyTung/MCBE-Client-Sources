
#include "CustomSky.h"

CustomSky::CustomSky() : Module("CustomSky", "CustomSky.", Category::RENDER) {
	registerSetting(new SliderSetting<float>("Intensity", "NULL", &intensity, 0.05f, 0.01f, 0.1f));
	registerSetting(new BoolSetting("Night", "NULL", &night, false));
	registerSetting(new BoolSetting("Custom", "NULL", &custom, false));
	registerSetting(new ColorSetting("Color", "NULL", &color, { 255, 255, 255 }));
}