#include "ClientTheme.h"

#include "../../../Utils/ClientColors.h"
ClientTheme::ClientTheme() : IModule(0, Category::CLIENT, "Go for a retro style or streamline.") {
	Theme
		.addEntry("Nuke.pl", 0)
		.addEntry("Frost", 1);
	registerEnumSetting("Client", &Theme, 0);
}

ClientTheme::~ClientTheme() {
}

const char* ClientTheme::getModuleName() {
	return ("ClientTheme");
}