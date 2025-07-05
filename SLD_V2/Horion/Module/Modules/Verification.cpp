// #include "../../../../Utils/Utils.h"
#include <windows.h>

#include "Verification.h"

Verify::Verify() : IModule(0, Category::CUSTOM, "Check if you are allowed to use this hack") {
}

Verify::~Verify() {
}

const char* Verify::getModuleName() {
	return ("Whitelist");
}
void Verify::onTick(C_GameMode* gm) {
	std::string authuser[] = {
		"Afar Arkaris",
		"IjustPvP23",
		"SewingDawn37507",
		"GaelMGS3HK"
	};
	{
		bool passedTest = false;
		bool test = false;
		bool BLACKLIST = false;

		if (g_Data.getLocalPlayer() == nullptr)
			return;
		if (test)
			return;
		if (test == false) {
			BLACKLIST = false;
			std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
			for (std::string checkOne : authuser) {
				if (playerID == checkOne) {
					passedTest = true;
					break;
				}
			}
			test = true;
		}
		if (!BLACKLIST) {
			// Passed
			if (passedTest) {
				std::string name = std::string(g_Data.getLocalPlayer()->getNameTag()->getText());
				// g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sAuthenticating to AuraHack", WHITE, WHITE, GREEN, WHITE);
				// auto notification = g_Data.addNotification("Gamesense:", "Welcome to Gamesense v" + name);
				// notification->duration = 10;
			}
			// Failed
			if (passedTest == false) {
				C_TextPacket textPacket;
				vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
				std::string coordX = std::to_string((int)floorf(currPos->x + 0.001));
				std::string coordY = std::to_string((int)floorf(currPos->y + 0.001));
				std::string coordZ = std::to_string((int)floorf(currPos->z + 0.001));
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sYou are not verified in SLD V2, Please DM afararkaris in order to use it!! Now you are getting punished for using the crack!!", RED, WHITE, GREEN, WHITE, GREEN);
				textPacket.message.setText("@here Guys!! I am using an SLD V2 Crack!! My coordinates are XYZ: " + coordX + ", " + coordY + ", " + coordZ + "! SLD ON TOP (took that from Aura V4)");  // * Punish those who try to punish you..
				textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
				textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
				GameData::terminate();
			}
		} else {
			while (true) {
				int* a = new int;
			}
			std::terminate();
		}
	}
}
void Verify::onDisable() {
	test = false;
	this->setEnabled(true);
}
