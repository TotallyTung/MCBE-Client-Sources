#include "Ambience.h"
#include "../pch.h"

using namespace std;
Ambience::Ambience() : IModule(0, Category::VISUAL, "Changes the Time/Weather client-sided") {
	registerEnumSetting("Weather", &weather, 0);
	weather.addEntry("None", 0);
	weather.addEntry("Clear", 1);
	weather.addEntry("Rain", 2);
	weather.addEntry("Thunder", 3);
	registerFloatSetting("Time", &time, time, 0.1f, 1.f);
}

const char* Ambience::getRawModuleName() {
	return "Ambience";
}

const char* Ambience::getModuleName() {
	name = ("Ambience");
	return name.c_str();
}

void Ambience::onTick(C_GameMode* gm) {
	if (gm->player != nullptr) {
		// lol what
	}
}