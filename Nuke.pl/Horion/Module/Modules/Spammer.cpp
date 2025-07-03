#include "Spammer.h"
Spammer::Spammer() : IModule(0, Category::MISC, "Spams a message in a specified delay.") {
	registerIntSetting("Delay", &delay, delay, 1, 10);
	registerIntSetting("Length", &length, length, 1, 60);
	registerBoolSetting("Bypass", &bypass, bypass);
}

Spammer::~Spammer() {
}

const char* Spammer::getModuleName() {
	return ("Spammer");
}
void Spammer::onTick(GameMode* gm) {
	Odelay++;
	if (Odelay > delay * 20) {
		Vec3* currPos = Game.getLocalPlayer()->getPos();
		std::string coordX = std::to_string((int)floorf(currPos->x + 0.001));
		std::string coordY = std::to_string((int)floorf(currPos->y + 0.001));
		std::string coordZ = std::to_string((int)floorf(currPos->z + 0.001));
		Utils::sendChat(bypass ? ("#" + Utils::getClientName() + "Client ###Purchase for " + Utils::getClientPrice() + " | " + Utils::randomString(length)) : "#" + Utils::getClientName() + "Client ###Purchase for " + Utils::getClientPrice());
		Odelay = 0;
	}
}
