#include "Notifications.h"
#include "../../../../Utils/ImGuiUtils.h"
#include <string>
#include <iomanip>
#include <sstream>

Notifications::Notifications() : IModule(0, Category::OTHER, "Displays notifications") {
	shouldHide = true;
	//mode.addEntry("None", 0, "Default modes");
	mode.addEntry("Underline", 1, "Underline modes");
	mode.addEntry("Fill", 2, "Fill modes");
	registerEnumSetting("Mode", &mode, 0, "Change modes");
	registerIntSetting("Scale", &notifSize, notifSize, 15, 40, "Scale of the notifications");
}

const char* Notifications::getRawModuleName() {
	return ("Notifications");
}
