#include "AutoDupe.h"
AutoDupe::AutoDupe() : IModule(0, Category::PLAYER, "/dupe") {
}
AutoDupe::~AutoDupe() {
}
const char* AutoDupe::getModuleName() {
	return ("AutoDupe");
};
void AutoDupe::onTick(GameMode* gm) {
	Utils::sendCommand("dupe");
}