#include "NoPacket.h"

NoPacket::NoPacket() : IModule(0, Category::MISC, "Prevents you from sending InventoryTransaction packets.") {
	registerBoolSetting("LevelSoundEvent", &SoundPacket, SoundPacket);
}

NoPacket::~NoPacket() {
}

const char* NoPacket::getModuleName() {
	return ("NoPacket");
}
bool NoPacket::allowAutoStart() {
	return false;
}
void NoPacket::onEnable() {
	if(!Game.isInGame())
		setEnabled(false);
}
