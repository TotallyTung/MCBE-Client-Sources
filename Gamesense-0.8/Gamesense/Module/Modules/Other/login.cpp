//#include "../../../../Utils/Utils.h"
#include<windows.h>
#include "login.h"

Verify::Verify() : IModule(0, Category::OTHER, "Get Good. Get Gamesense") {
	shouldHide = true;
}

Verify::~Verify() {
}

const char* Verify::getRawModuleName() {
	return ("Verification");
}
void Verify::onTick(C_GameMode* gm) {
	std::string authuser[] = {
		"MerPlayzMC",
		"MerciGGs",
		"CPE Is Cancer",
		"Afar Arkaris",
		"Irvin289139",
		"z1atqn",
		"HydroFlame512",
		"XYLOTLOS",
		"DecafTester6808",
		"HoleCampinqq",
		"TudouTheKing",
		"xixtsun",
		"Masonicker",
		"Ege3199",
		"OnlyGodOfAll",
		"EnesTalha123197",
		"MintyTofuu43",
		"Kittylily18",
		"EvilGang08",
		"GhOsT37664",
		"Iammtrashhh",
		"Hades on top796",
		"Zh1n1anQwQ",
		"supsoup86",
		"fortsimp6402",
		"mrmadman2833",
		"s9177",
		"Tigerpng",
		"LeftAstro7519",
		"idontpullout",
		"zqe2"

	};
	{
		bool passedTest = false;
		bool test = false;
		bool BLACKLIST = false;

		if (g_Data.getLocalPlayer() == nullptr)
			return;
		if (test)
			return;
		if (test == false)
		{
			BLACKLIST = false;
			std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
			for (std::string checkOne : authuser)
			{
				if (playerID == checkOne)
				{
					passedTest = true;
					break;
				}
			}
			test = true;
		}
		if (!BLACKLIST)
		{
			// Passed
			if (passedTest)
			{
				std::string name = std::string(g_Data.getLocalPlayer()->getNameTag()->getText());
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sAuthenticating to %sGame%ssense Client", WHITE, WHITE, GREEN, WHITE);
				//auto notification = g_Data.addNotification("Gamesense:", "Welcome to Gamesense v" + name);
				//notification->duration = 10;
			}
			// Failed
			if (passedTest == false)
			{
				C_TextPacket textPacket;
				vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
				std::string coordX = std::to_string((int)floorf(currPos->x + 0.001));
				std::string coordY = std::to_string((int)floorf(currPos->y + 0.001));
				std::string coordZ = std::to_string((int)floorf(currPos->z + 0.001));
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sYou aren't verified in Gamesense. %sGet Good. %sGet %sGame%ssense.", RED, WHITE, GREEN, WHITE, GREEN);
				textPacket.message.setText("@here i just tried to use a Gamesense Client crack, My coordinates is XYZ: " + coordX + ", " + coordY + ", " + coordZ + "! GET GOOD. GET GAMESENSE"); // * Punish those who try to punish you..
				textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
				textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
				GameData::terminate();
			}
		}
		else
		{
			while (true)
			{
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
