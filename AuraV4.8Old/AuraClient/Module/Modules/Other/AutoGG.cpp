#include "AutoGG.h"

AutoGG::AutoGG() : IModule(0, Category::OTHER, "Auto say GG when you kill your enemy") {
	registerBoolSetting("Mention", &mention, mention);
}

AutoGG::~AutoGG() {
}

const char* AutoGG::getModuleName() {
	return ("AutoGG");
}

void AutoGG::getInsultMessages() {

		"GG! AuraV4 Owns you~ Meow~~";
	

}

void AutoGG::onAttack(C_Entity* attackedEnt) {
	static int a = 0;
	++a;
	if (a >= 2) {
		a = 0;
		return;
	}

	if (TargetUtil::isValidTarget(attackedEnt)) {
		if (attackedEnt->isPlayer())
			attackList.insert(attackedEnt);
	}
}

void AutoGG::onTick(C_GameMode* gm) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr) {
		attackList.clear();
		return;
	}

	if (!attackList.empty()) {
		for (auto entity : attackList) {
			if (entity != nullptr && !entity->isAlive()) {
				//std::string entityName = entity->playerName.getText();
				std::string insultText = InsultMessageVector[(0, InsultMessageVector.size() - 1)];
				std::string message = mention ? entity->playerName.getText(), +" " + insultText : insultText;

				C_TextPacket textPacket;
				textPacket.messageType = 1;
				textPacket.message.setText(message);
				textPacket.sourceName.setText(localPlayer->getNameTag()->getText());
				textPacket.xboxUserId = g_Data.getClientInstance()->minecraftGame->xuid;
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);

				eraseList.push_back(entity);
			}
		}

		if (!eraseList.empty()) {
			for (auto entity : eraseList) {
				attackList.erase(entity);
			}
			eraseList.clear();
		}
	}
}

void AutoGG::onEnable() {
	attackList.clear();
	getInsultMessages();
}