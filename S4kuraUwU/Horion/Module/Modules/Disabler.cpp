#include "Disabler.h"

Disabler::Disabler() : IModule('0', Category::MISC, "Disabler test") {

}

const char* Disabler::getModuleName() {
	return ("Disabler");
}

void Disabler::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (this->elytra && g_Data.isInGame()) {
		C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
		C_PlayerActionPacket p;
		p.action = 16;  //15
		p.entityRuntimeId = g_Data.getLocalPlayer()->entityRuntimeId;
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
	}
}

void Disabler::onPlayerTick(C_Player* plr) {
	plr->pitch = 270.f;
}
