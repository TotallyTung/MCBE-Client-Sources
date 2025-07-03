#include "Crasher.h"

Nuker::Nuker() : Module("Crasher", "Break multiple blocks at once.", Category::WORLD) {
	registerSetting(new SliderSetting<int>("Radius", "The Size", &nukerRadius, nukerRadius, 1, 10));
}

Nuker::~Nuker() {
}


void Nuker::onNormalTick(LocalPlayer* gm) {
	bool outBool = false;
	Vec3<int> tempPos = gm->getPos().toInt();
	for (int x = -nukerRadius; x < nukerRadius; x++) {
		for (int y = -nukerRadius; y < nukerRadius; y++) {
			for (int z = -nukerRadius; z < nukerRadius; z++) {
				tempPos.x += x;
				tempPos.y += y;
				tempPos.z += z;
				if (tempPos.y > -64 && Game.getClientInstance()->getRegion()->getBlock(tempPos)->blockLegacy->isSolid()) {
					gm->gameMode->destroyBlock(tempPos, 1);
				}
			}
		}
	}
}
