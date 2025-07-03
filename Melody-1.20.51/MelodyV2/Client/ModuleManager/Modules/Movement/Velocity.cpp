// Created by Tony on 2024-10-10 10:01:54

#include "Velocity.h"

Velocity::Velocity() : Module("Velocity", "Don't ever take knockback again.", Category::MOVEMENT) {
	addSlider<float>("XZ Modifier", "Changes x & z velocity.", ValueType::FLOAT_T, &xzModifier, 0.f, 1.f);
	addSlider<float>("Y Modifier", "Changes y velocity.", ValueType::FLOAT_T, &yModifier, 0.f, 1.f);
}

std::string Velocity::getModName() {
	static char textStr[15];
	sprintf_s(textStr, 15, "H%i%%V%i%%", (int)(xzModifier * 100), (int)(yModifier * 100));
	return std::string(textStr);
}
