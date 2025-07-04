#include "Notifications.h"

Notifications::Notifications() : IModule(0, Category::VISUAL, "Gives Notifications For Disabling/Enabling Modules") {
}

Notifications::~Notifications() {
}

const char* Notifications::getModuleName() {
	return ("Notifications");
}

