#include "Center.h"

using namespace std;
Center::Center() : IModule(0, Category::MOVEMENT, "Hole Swaps") {
}

const char* Center::getModuleName() {
	return ("Center");
}

void Center::onTick(GameMode* gm) {
	Vec3 yR = Vec3(floor(Game.getLocalPlayer()->getPos()->x), Game.getLocalPlayer()->getPos()->y, floor(Game.getLocalPlayer()->getPos()->z));
	yR.x += 0.5f;
	yR.z += 0.5f;
	Game.getLocalPlayer()->setPos(yR);
	moduleMgr->getModule<Center>()->setEnabled(false);
}