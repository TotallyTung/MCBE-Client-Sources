#include "InstaBreak.h"

InstaBreak::InstaBreak() : IModule(VK_NUMPAD4, Category::WORLD, "Break any block instantly") {
	Modes = SettingEnum(this)
				.addEntry(EnumEntry("Instant", 0))
				.addEntry(EnumEntry("Packet", 1))
				.addEntry(EnumEntry("Haste", 2));
	registerEnumSetting("Modes", &this->Modes, 0);

	registerIntSetting("Haste Delay", &delay, delay, 0, 20);
}

InstaBreak::~InstaBreak() {
}

const char* InstaBreak::getModuleName() {
	return ("InstaBreak");
}