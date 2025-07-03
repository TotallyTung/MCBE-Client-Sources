#include "Velocity.h"

Velocity::Velocity() : IModule(0, Category::MOVEMENT, "Don't ever take knockback again.") {
	registerFloatSetting("Linear Modifier", &xModifier, xModifier, 0.f, 1.f);
	registerFloatSetting("Height Modifier", &yModifier, yModifier, 0.f, 1.f);
}

Velocity::~Velocity() {
}

const char* Velocity::getModuleName() {
	return ("Velocity");
}

const char* Velocity::getModName() {
	static char xStr[5];
	snprintf(xStr, 5, "H%i", (int)(xModifier * 100));

	static char yStr[5];
	snprintf(yStr, 5, "V%i", (int)(yModifier * 100));
	std::string name = xStr + std::string("%") + yStr + std::string("%");
	return name.c_str();
}
