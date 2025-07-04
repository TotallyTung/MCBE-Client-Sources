#include "Clients.h"
#include "../../../Client.h"

Clients::Clients() : Module("Clients", "For clients Purpose", Category::CLIENT) {
	addClickMeSetting("Save config", "save the current config");
}
Clients::~Clients() {
}

bool Clients::isEnabled() {
	return true;
}
bool Clients::isVisible() {
	return false;
}

