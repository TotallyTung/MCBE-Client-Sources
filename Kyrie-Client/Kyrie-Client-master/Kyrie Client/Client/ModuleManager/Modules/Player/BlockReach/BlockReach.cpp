#include "BlockReach.h"

BlockReach::BlockReach() : Module(0, Category::PLAYER, nullptr) {
	registerFloatSetting("Reach", &blockReach, blockReach, 5.f, 20.f);
}

const char* BlockReach::getModuleName() {
	return "BlockReach";
}