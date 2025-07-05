#include "AutoEZ.h"

#include <stdlib.h> /* srand, rand */

#include "../../../Utils/Utils.h"

AutoEZ::AutoEZ() : IModule(0, Category::COMBAT, "troll enemies and anger them2") {
	//registerBoolSetting("Lose Message", &this->sayUserDied, this->sayUserDied);
	registerBoolSetting("AutoEZ", &this->sayEnemDied, this->sayEnemDied);
	registerBoolSetting("AutoGG For Yiu", &this->sayEnemDiedGG, this->sayEnemDiedGG);
}

AutoEZ::~AutoEZ() {}

const char* AutoEZ::getModuleName() {
	return ("AutoEZ");
}

static std::vector<C_Entity*> targetList;

void findEnemyPlayer(C_Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr)
		return;  // something i just learned: return = terminate func

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (currentEntity->getEntityTypeId() != 319)  // player
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 7) {
		targetList.push_back(currentEntity);
	}
}  // thanks badman

void AutoEZ::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	targetList.clear();

	g_Data.forEachEntity(findEnemyPlayer);

	if (!targetList.empty() && g_Data.isInGame() && sayEnemDied) {  // Send kill messages
		if (GameData::canUseMoveKeys()) {
			// kill msg == switch back at 4  ...

			for (C_Entity* tgt : targetList) {

				if (targetList[0]->isAlive() && targetList[0]->height > 1.5f && targetList[0]->damageTime > 1) {
					foundPlayer = true;
				}

				if (foundPlayer && !tgt->isAlive() &&  !tgt->canShowNameTag() && tgt->getDeathTime() >= 0 &&  tgt->getNameTag()->getTextLength() > 1) {
					isDead = true;
				}

				if (isDead) {
					C_TextPacket textPacket;
					textPacket.message.setText(">Ezzzzzzzzzzzzz #TeamS4kuraUwU");
					textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
					textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
					foundPlayer = false;
					isDead = false;
				}
			}
		}
	}
	if (!targetList.empty() && g_Data.isInGame() && sayEnemDiedGG) {  // Send kill messages
		if (GameData::canUseMoveKeys()) {
			// kill msg == switch back at 4  ...

			for (C_Entity* tgt : targetList) {
				if (targetList[0]->isAlive() && targetList[0]->height > 1.5f && targetList[0]->damageTime > 1) {
					foundPlayer = true;
				}

				if (foundPlayer && !tgt->isAlive() && !tgt->canShowNameTag() && tgt->getDeathTime() >= 0 && tgt->getNameTag()->getTextLength() > 1) {
					isDead = true;
				}

				if (isDead) {
					C_TextPacket textPacket;
					textPacket.message.setText(">GG! Meow~~~~~~~~~ #TeamS4kuraUwU");
					textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
					textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
					foundPlayer = false;
					isDead = false;
				}
			}
		}
	}
	 if (targetList.empty()) {
		foundPlayer = false;
		isDead = false;
	}
	
	if (antiSpamCheck == false) {
		if ((g_Data.getLocalPlayer()->isAlive()) == false) {
			antiSpamCheck = false;
		}
	}
}