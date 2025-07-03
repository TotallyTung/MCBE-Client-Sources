#include "ChatSuffix.h"

#include <stdlib.h> /* srand, rand */
#include "../../ModuleManager.h"
bool whyDead = true;
// #include "../../Utils/Utils.h"
using namespace std;
ChatSuffix::ChatSuffix() : IModule(0x0, Category::OTHER, "More chat options")
{
	registerBoolSetting("Random String", &bypass, bypass);
	registerBoolSetting(">", &greenText, greenText, "Adds a > before ur message");
	registerBoolSetting("AutoEZ", &autoez, autoez, "Be toxic to enemy after killin them");
	// registerBoolSetting("Death Analyze", &whyDead, whyDead, "Analyzes why you died, like Future Client!");
	//  TODO: AutoExcuse, use same logic as auto ez but for u instead
	//  registerBoolSetting("AutoExcuse", &autoexcuse, autoexcuse);
	registerEnumSetting("Suffix", &mode, 0);
	mode.addEntry("None", 0);
	mode.addEntry("Random", 1);
	mode.addEntry("Fixed", 2);
	mode.addEntry("Version", 3);
	mode.addEntry("Custom", 4);
	shouldHide = true;
}

const char *ChatSuffix::getRawModuleName()
{
	return ("Chat");
};
static std::vector<C_Entity *> targetList;

void findEnemyPlayer(C_Entity *currentEntity, bool isRegularEntity)
{
	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())
		return;

	if (currentEntity->getEntityTypeId() != 319)
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 12)
	{
		targetList.push_back(currentEntity);
	}
}

void ChatSuffix::onTick(C_GameMode *gm)
{
	auto player = g_Data.getLocalPlayer();
	auto chat = moduleMgr->getModule<ChatSuffix>();
	if (player == nullptr)
		return;
	bool alive;
	bool iDied;

	targetList.clear();

	g_Data.forEachEntity(findEnemyPlayer);

	if (!g_Data.getLocalPlayer()->isAlive() && g_Data.isInGame())
	{
		if (GameData::canUseMoveKeys())
		{
			for (C_Entity *tgt : targetList)
			{
				if (g_Data.getLocalPlayer()->isAlive())
				{
					alive = true;
				}

				if (alive && !g_Data.getLocalPlayer()->isAlive())
				{
					iDied = true;
				}
				if (iDied && whyDead)
				{
					auto getPing = g_Data.getClientInstance()->getServerPingTime();
					std::string myPing = std::to_string(getPing);
					g_Data.getClientInstance()->getGuiData()->displayClientMessageF("<%sGame%ssense%s> %sDeath due to possible server latency of %s %scausing Totem Failure, Server Anticheat, or there are no totems in your inventory", WHITE, GREEN, WHITE, DARK_RED, myPing, DARK_RED);
					alive = false;
					iDied = false;
				}
			}
		}
	}
	if (g_Data.getLocalPlayer()->isAlive())
	{
		alive = false;
		iDied = false;
	}
	if (!targetList.empty() && g_Data.isInGame())
	{ // Send kill messages
		if (GameData::canUseMoveKeys())
		{

			for (C_Entity *tgt : targetList)
			{
				kc = rand() % 24;

				if (tgt->isAlive() && tgt->height > 1.5f)
				{
					foundPlayer = true;
				}

				if (foundPlayer && !tgt->isAlive() && /* !tgt->canShowNameTag() && tgt->getDeathTime() >= 0 && */ tgt->getNameTag()->getTextLength() > 1)
				{
					isDead = true;
				}
				if (isDead && chat->autoez) // aint no fucken way i didnt notice tht autoez wasnt optional for months LOL
				{
					int messageIndex = 0;
					if (messageIndex <= 0)
						messageIndex = 1; // Prevent invis msgs

					if (messageIndex > 13)
						messageIndex = 0;
					// string name = targetList[0]->getNameTag()->getText();
					string name = tgt->getNameTag()->getText(); // betaa
					name = Utils::sanitize(name);				// Name will say the name of the user killed
					auto ping = "@" + name;						// Ping will say @Steve
					std::string killMsg[24] = {
						ping + " has been EZ'd by Gamesense!",
						ping + " has been assraped by Gamesense!",
						ping + " has been crushed by my big balls",
						ping + " just got owned by gamesense!",
						"Get Good " + ping + " Get Gamesense",
						ping + ", can you see the Light?",
						ping + " just got dunked on by Gamesense",
						"GGEZ " + ping + " Gamesense on TOP",
						ping + " must be using a protohax rename",
						ping + " must be using toolbox",
						ping + " must be using horion",
						ping + " just got naynayed by Gamesense!",
						ping + " gamesense winning allday!",
						ping + " so ez for gamesense jajajajajjaja",
						ping + " ew get that nn client outta here",
						"nice try " + ping + " but gamesense still owns you",
						ping + " just got curbstomped by Gamesense",
						ping + " you were never going to win :(",
						ping + " must be using victory++",
						"kys " + ping + " just like i killed you here",
						"shittyy client " + ping + " get good get gamesense",
						ping + " if you wanna get good gotta get gamesense!",
						"whats 50meters wide and cant swim? " + ping + ".",
						ping + " just died in the most embarrassing way possible LOL"};
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
	if (targetList.empty())
	{
		foundPlayer = false;
		isDead = false;
	}
	if (antiSpamCheck == false)
	{
		if ((g_Data.getLocalPlayer()->isAlive()) == true)
		{
			antiSpamCheck = true;
		}
	}
}

void ChatSuffix::onSendPacket(C_Packet *packet)
{
	std::string end;
	std::string end2;
	std::string end3;
	auto interfaceMod = moduleMgr->getModule<Interface>();
	if (packet->isInstanceOf<C_TextPacket>())
	{
		C_TextPacket *funy = reinterpret_cast<C_TextPacket *>(packet);
		std::string Sentence;
		if (mode.selected == 1)
		{
			switch (rand() % 10)
			{
			case 0:
				end = " | Gamesense";
				break;
			case 1:
				end = " | GS";
				break;
			case 2:
				end = " | Gamesense.pub";
				break;
			case 3:
				end = " | Get Gamesense";
				break;
			case 4:
				end = " | Gamesense v" + interfaceMod->versionStr;
				break;
			}
		}
		Sentence = funy->message.getText();
		if (bypass)
		{
			static const char alphanum[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			std::string tmp_s;
			tmp_s.reserve(6);

			for (int i = 0; i < 6; ++i)
			{
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
				end2 = " | Gamesense";
			Sentence = Sentence + end2;

			// Show version
			if (mode.getSelectedValue() == 3)
				end3 = " | Gamesense v" + interfaceMod->versionStr;
			Sentence = Sentence + end3;

			// Custom
			// if (mode.getSelectedValue() == 3)
			// custom;
			// Sentence = Sentence + " | " + custom;
		}
		if (greenText)
		{
			Sentence = "> " + Sentence;
		}
		funy->message.resetWithoutDelete();
		funy->message.setText(Sentence);
	}
}
