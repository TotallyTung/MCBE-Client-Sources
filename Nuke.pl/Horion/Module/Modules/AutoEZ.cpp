#include "AutoEZ.h"

AutoEZ::AutoEZ() : IModule(0, Category::MISC, "troll enemies and anger them2") {
	registerEnumSetting("MessageType", &MsgType, 0);
	MsgType.addEntry("Chat", 0);
	MsgType.addEntry("Tell", 1);
	registerBoolSetting("Lose Message", &this->sayUserDied, this->sayUserDied);
	registerBoolSetting("Win Message", &this->sayEnemDied, this->sayEnemDied);
}

AutoEZ::~AutoEZ() {}

const char* AutoEZ::getModuleName() {
	return ("AutoEZ");
}

static std::vector<Entity*> targetList;

void findEnemyPlayer(Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr)
		return;  // something i just learned: return = terminate func

	if (currentEntity == Game.getLocalPlayer())  // Skip Local player
		return;

	if (currentEntity->getEntityTypeId() != 319)  // player
		return;

	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());

	if (dist < 12) {
		targetList.push_back(currentEntity);
	}
}  // thanks badman

void AutoEZ::onTick(GameMode* gm) {
	targetList.clear();

	if (Game.getLocalPlayer() == nullptr)
		return;
	if (!Game.canUseMoveKeys())
		return;

	Game.forEachPlayer(findEnemyPlayer);

	if (!targetList.empty() && Game.isInGame() && sayEnemDied) {
		if (GameData::canUseMoveKeys()) {
			// kill msg == switch back at 4  ...

			for (Entity* tgt : targetList) {

				if (tgt->isAlive() && tgt->height > 1.5f) {
					foundPlayer = true;
				}

				if (foundPlayer && !tgt->isAlive() && /* !tgt->canShowNameTag() && tgt->getDeathTime() >= 0 && */ tgt->getNameTag()->getTextLength() > 1) {
					isDead = true;
				}

				if (isDead) {
					std::string ZaridMember[] = {
						"GiveMeCrystal",
						"nukupl",
						"ReaQwQDayo",
						"NukeplDev",
						"Okome1226",
						"TheCpePolice",
						"ConfigLoad98",
						"cyom1025"
					};
					bool member;
					for (auto checkOne : ZaridMember) {
						if (tgt->getNameTag()->getText() == checkOne) {
							member = true;
						}
					}
					if (member) {
						Utils::sendChat("@" + std::string(tgt->getNameTag()->getText()) + " EZZZZ Get #" + Utils::getClientName() + " For " + Utils::getClientPrice());
						clientMessageF("%s@%s Died After Popping 0 totems", Utils::getCommandClientName(), tgt->getNameTag()->getText());
					}
					else if (!member) {
						Utils::sendChat("@" + std::string(tgt->getNameTag()->getText()) + " Sorry " +Utils::getClientName() + "Member, Nice Try!");
						clientMessageF("%s@%s Died After Popping 0 totems (%sMember)", Utils::getCommandClientName(), tgt->getNameTag()->getText(), Utils::getClientName());
					}
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

	if (Game.isInGame() && sayUserDied && antiSpamCheck) {
		if ((Game.getLocalPlayer()->isAlive()) == false) {
			Utils::sendChat("GG Good Client/PS | @" + std::string(targetList[0]->getNameTag()->getText()));
			antiSpamCheck = false;
		}
	}

	if (antiSpamCheck == false) {
		if ((Game.getLocalPlayer()->isAlive()) == true) {
			antiSpamCheck = true;
		}
	}
}
