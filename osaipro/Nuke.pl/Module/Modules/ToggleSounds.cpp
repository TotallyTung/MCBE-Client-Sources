#include "ToggleSounds.h"
ToggleSounds::ToggleSounds() : IModule(0, Category::MISC, "Plays a sound when you toggle a module") {
	registerEnumSetting("Sound", &sound, 0);
	sound.addEntry("Sigma", 0);
	sound.addEntry("Tadokoro", 1);
}

const char* ToggleSounds::getModuleName() {
	return "ToggleSounds";
}