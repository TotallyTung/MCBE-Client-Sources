#include "Velocity.h"
#include "../pch.h"

using namespace std;
Velocity::Velocity() : IModule(0, Category::MOVEMENT, "Modifies your velocity (knockback)") {
	registerFloatSetting("Linear", &x, x, 0.f, 2.f);
	registerFloatSetting("Height", &y, y, 0.f, 2.f);
}

const char* Velocity::getRawModuleName() {
	return "Velocity";
}

const char* Velocity::getModuleName() {
	//name = string("Velocity ") + string(GRAY) + to_string((int)x) + string(".") + to_string((int)(x * 10) - ((int)x * 10));
	//fullname = name + string("% ") + to_string((int)y) + string(".") + to_string((int)(y * 10) - ((int)y * 10)) + string("%");
	//fullname = name + string("% ") + to_string((int)y) + string(".") + to_string((int)(y * 10) - ((int)y * 10)) + string("%");
	fullname = string("Velocity ") + string(GRAY) + string("[") + string(WHITE) + to_string((int)x) + string(".") + to_string((int)(x * 10) - ((int)x * 10)) + string("%") + string(GRAY) + string("|") + string(WHITE) + to_string((int)y) + string(".") + to_string((int)(y * 10) - ((int)y * 10)) + string("%") + string(GRAY) + string("]");
	return fullname.c_str();
}

void Velocity::onTick(C_GameMode* gm) {
}