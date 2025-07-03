#include "InstaBreak.h"

InstaBreak::InstaBreak() : IModule(0, Category::WORLD, "Break any block instantly.") {
	registerBoolSetting("WookMC", &wookmc, true);
}

InstaBreak::~InstaBreak() {
}

const char* InstaBreak::getModuleName() {
	return ("InstaBreak");
}

void InstaBreak::onMouseClickUpdate(int key, bool isDown) {
	if (key == 1 && isDown && moduleMgr->getModule<InstaBreak>()->wookmc && moduleMgr->getModule<InstaBreak>()->isEnabled()) {
		Vec3i BlockPos = Game.getLocalPlayer()->getlevel()->block;
		bool awa = true;
		Game.getLocalPlayer()->getGameMode()->destroyBlock(&BlockPos, 0);
	}
}