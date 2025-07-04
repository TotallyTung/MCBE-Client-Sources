#include "Unject.h"
#include "../../../../../Client/Client.h"

Unject::Unject() : Module(0x0, Category::CLIENT, "Unject client") {
}

const char* Unject::getModuleName() {
	return "Unject";
}

bool Unject::isVisible() {
	return false;
}

void Unject::onEnable() {
	client->unject();
}