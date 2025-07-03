#include "AutoEZ.h"
AutoEZ::AutoEZ() : IModule(0x0, Category::MISC, "More chat options") {
}

const char* AutoEZ::getModuleName() {
	return ("AutoEZ");
};
void AutoEZ::onTick(GameMode* gm) {
}

void AutoEZ::onJoiningServer() {
}