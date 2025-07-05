#include "Notifications.h"

Notifications::Notifications() : IModule(0, Category::MISC, "Gives Notifications For Disabling/Enabling Modules") {
}

Notifications::~Notifications() {
}

const char* Notifications::getModuleName() {
	return ("Notifications");
}

