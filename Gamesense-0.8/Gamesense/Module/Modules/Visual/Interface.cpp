#include "Interface.h"

Interface::Interface() : IModule(0, Category::OTHER, "Change the color of the Client")
{
	registerEnumSetting("Mode", &color, 0);
	color.addEntry("Wave", 0);
	color.addEntry("RGB", 1);
	color.addEntry("Astolfo", 2);
	color.addEntry("Horion", 4);
	// color.addEntry("RGBWave", 3);
	registerFloatSetting("Brightness", &brightness, brightness, 0.f, 1.f);
	registerFloatSetting("Saturation", &saturation, saturation, 0.f, 1.f);
	registerIntSetting("Spacing", &spacing, spacing, 1, 200);
	registerFloatSetting("Opacity", &coloropacity, coloropacity, 0.f, 255.f);
	registerFloatSetting("R", &r, r, 0.f, 255.f);
	registerFloatSetting("G", &g, g, 0.f, 255.f);
	registerFloatSetting("B", &b, b, 0.f, 255.f);
	registerFloatSetting("R2", &r2, r2, 0.f, 255.f);
	registerFloatSetting("G2", &g2, g2, 0.f, 255.f);
	registerFloatSetting("B2", &b2, b2, 0.f, 255.f);
	shouldHide = true;
}

const char *Interface::getRawModuleName()
{
	return ("Colors");
}