#include "ChatSuffix.h"

#include <stdlib.h> /* srand, rand */

#include "../ModuleManager.h"

bool whyDead = true;
// #include "../../Utils/Utils.h"
using namespace std;
ChatSuffix::ChatSuffix() : IModule(0x0, Category::MISC, "More chat options") {
	registerBoolSetting("Random String", &bypass, bypass);
	registerBoolSetting(">", &greenText, greenText);
	registerBoolSetting("AutoEZ", &autoez, autoez);
	// registerBoolSetting("Death Analyze", &whyDead, whyDead, "Analyzes why you died, like Future Client!");
	//  TODO: AutoExcuse, use same logic as auto ez but for u instead
	//  registerBoolSetting("AutoExcuse", &autoexcuse, autoexcuse);
	registerEnumSetting("Suffix", &mode, 0);
	mode.addEntry(EnumEntry("None", 0));
	mode.addEntry(EnumEntry("Random", 1));
	mode.addEntry(EnumEntry("Fixed", 2));
	mode.addEntry(EnumEntry("Version", 3));
	mode.addEntry(EnumEntry("Custom", 4));
}

const char *ChatSuffix::getModuleName() {
	return ("Chat");
};
static std::vector<C_Entity *> targetList;

void findEnemyPlayer(C_Entity *currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())
		return;

	if (currentEntity->getEntityTypeId() != 319)
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 12) {
		targetList.push_back(currentEntity);
	}
}

void ChatSuffix::onTick(C_GameMode *gm) {
	auto player = g_Data.getLocalPlayer();
	auto chat = moduleMgr->getModule<ChatSuffix>();
	if (player == nullptr)
		return;
	bool alive;
	bool iDied;

	targetList.clear();

	g_Data.forEachEntity(findEnemyPlayer);

	if (!g_Data.getLocalPlayer()->isAlive() && g_Data.isInGame()) {
		if (GameData::canUseMoveKeys()) {
			for (C_Entity *tgt : targetList) {
				if (g_Data.getLocalPlayer()->isAlive()) {
					alive = true;
				}

				if (alive && !g_Data.getLocalPlayer()->isAlive()) {
					iDied = true;
				}
				if (iDied && whyDead) {
					auto getPing = g_Data.getClientInstance()->getServerPingTime();
					std::string myPing = std::to_string(getPing);
					g_Data.getClientInstance()->getGuiData()->displayClientMessageF("<%sSLD%s> %sDeath due to no skill", WHITE, GREEN, WHITE, DARK_RED, myPing, DARK_RED);
					alive = false;
					iDied = false;
				}
			}
		}
	}
	if (g_Data.getLocalPlayer()->isAlive()) {
		alive = false;
		iDied = false;
	}
	if (!targetList.empty() && g_Data.isInGame()) {  // Send kill messages
		if (GameData::canUseMoveKeys()) {
			for (C_Entity *tgt : targetList) {
				kc = rand() % 24;

				if (tgt->isAlive() && tgt->height > 1.5f) {
					foundPlayer = true;
				}

				if (foundPlayer && !tgt->isAlive() && /* !tgt->canShowNameTag() && tgt->getDeathTime() >= 0 && */ tgt->getNameTag()->getTextLength() > 1) {
					isDead = true;
				}
				if (isDead && chat->autoez)  // aint no fucken way i didnt notice tht autoez wasnt optional for months LOL
				{
					int messageIndex = 0;
					if (messageIndex <= 0)
						messageIndex = 1;  // Prevent invis msgs

					if (messageIndex > 13)
						messageIndex = 0;
					// string name = targetList[0]->getNameTag()->getText();
					string name = tgt->getNameTag()->getText();  // betaa
					name = Utils::sanitize(name);                // Name will say the name of the user killed
					auto ping = "@" + name;                      // Ping will say @Steve
					std::string killMsg[3] = {
						ping + " EZZZZZZZZZZZZZZZZZZZZ",
						ping + " Why are you so weak? Killed by Team SLD",
						ping + " SLD OWNS YOU"
						};
					C_TextPacket textPacket;
					textPacket.message.setText(killMsg[kc]);
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
		if ((g_Data.getLocalPlayer()->isAlive()) == true) {
			antiSpamCheck = true;
		}
	}
}

void ChatSuffix::onSendPacket(C_Packet *packet) {
	std::string end;
	std::string end2;
	std::string end3;
	//auto interfaceMod = moduleMgr->getModule<Interface>();
	if (packet->isInstanceOf<C_TextPacket>()) {
		C_TextPacket *funy = reinterpret_cast<C_TextPacket *>(packet);
		std::string Sentence;
		if (mode.selected == 1) {
			switch (rand() % 10) {
			case 0:
				end = " | SOLDIERS";
				break;
			case 1:
				end = " | SLD";
				break;
			case 2:
				end = " | SLD.Client";
				break;
			case 3:
				end = " | Join Soldiers";
				break;
			case 4:
				end = " | SLD V2";
				break;
			}
		}
		Sentence = funy->message.getText();
		if (bypass) {
			static const char alphanum[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			std::string tmp_s;
			tmp_s.reserve(6);

			for (int i = 0; i < 6; ++i) {
				tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
			}
			tmp_s = " | " + tmp_s;

			Sentence += tmp_s;
		}
		{
			// Random
			if (mode.getSelectedValue() == 1)
				Sentence += end;

			// Fixed
			if (mode.getSelectedValue() == 2)
				end2 = " | Soldiers";
			Sentence = Sentence + end2;

			// Show version
			if (mode.getSelectedValue() == 3)
				end3 = " | Soldiers V2";
			Sentence = Sentence + end3;

			// Custom
			// if (mode.getSelectedValue() == 3)
			// custom;
			// Sentence = Sentence + " | " + custom;
		}
		if (greenText) {
			Sentence = "> " + Sentence;
		}
		funy->message.resetWithoutDelete();
		funy->message.setText(Sentence);
	}
}
