#include "Kit.h"
#include "../ModuleManager.h"
bool DropAll = false;
Kit::Kit() : IModule(0, Category::PLAYER, "/Kit | OverWorld: 0 |&| Nether: 1") {
	registerBoolSetting("DropInventory", &DropAll, false);
}
Kit::~Kit() {
}
const char* Kit::getModuleName() {
	return ("AutoKit");
};
void Kit::onTick(GameMode* gm) {
	if (Game.getLocalPlayer() == nullptr) return;
	if (!Game.canUseMoveKeys()) return;
	if (DropAll)  Game.getLocalPlayer()->getSupplies()->inventory->dropAll();
	int dimension = 0;
	Game.getLocalPlayer()->getDimensionId(&dimension);
	if (dimension == 0) {
		Utils::sendCommand("kit 1");
	}
	if (dimension == 1) {
		Utils::sendCommand("kit 2");
	}
	this->setEnabled(false);
}