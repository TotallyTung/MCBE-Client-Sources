#include "hub.h"
hub::hub() : IModule(0, Category::PLAYER, "/hub") {
}
hub::~hub() {
}
const char* hub::getModuleName() {
	return ("sendHub");
};
void hub::onTick(GameMode* gm) {
	if (Game.getLocalPlayer() == nullptr) return;
	if (!Game.canUseMoveKeys()) return;
	Utils::sendCommand("hub");
	this->setEnabled(false);
}