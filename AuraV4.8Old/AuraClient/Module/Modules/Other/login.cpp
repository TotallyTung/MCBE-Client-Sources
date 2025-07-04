//#include "../../../../Utils/Utils.h"
#include<windows.h>
#include <playsoundapi.h>
#include "login.h"

Verify::Verify() : IModule(0, Category::OTHER, "Check if you are allowed to use this hack") {
	shouldHide = true;
}

Verify::~Verify() {
}

const char* Verify::getModuleName() {
	return ("Whitelist");
}
void Verify::onTick(C_GameMode* gm) {
	std::string authuser[] = {
		"Afar Arkaris",
		"AURA taken",
		"shu01828371",
		"killaura7787",
		"KiarawaT",
		"KANAMARUDAU",
		"ponta5399",
		"Man Lay214",
		"sagiricuteee",
		"LanGameYT",
		"RTX 4070TiWhite",
		"S4kuraUwUV2",
		"DsY Emilia",
		"Zh1n1anQwQ",
		"meow Yiu0704",
		"IronSatyr706872",
		"AuraOwnsAll",
		"Nakeanl",
		"SlimyMoney27202",
		"MoonSakura6738",
		"naninaka",
		"Awarevenger",
		"William961130",
		"Jiasj1ajiX",
		"NightSquad3076",
		"ReaQwQDayo",
		"hati Demu",
		"yuu10392084",
		"hackermc9816",
		"Fusei12163",
	    "CrumplyMoss6467",
		"ApsidalPine389",
		"yopiyopiV1",
		"Iammtrashhh",
		"ApplePmeow",
		"xIhyna",
		"KingCrownOG",
		"RyZeper",
		"DualNebul090112",
		"Fr0stb0rneAlpha",
		"Jiuweihu1885",
		"FYRE5963"
	};
	{
		passedTest = false;
		test = false;
		if (g_Data.getLocalPlayer() == nullptr)
			return;
		if (test)
			return;
		if (test == false)
		{
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
		// Failed
		if (passedTest == false)
		{
			C_TextPacket textPacket;
			vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
			std::string coordX = std::to_string((int)floorf(currPos->x + 0.001));
			std::string coordY = std::to_string((int)floorf(currPos->y + 0.001));
			std::string coordZ = std::to_string((int)floorf(currPos->z + 0.001));
			g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sYou are not verified in AuraHack, Please DM afararkaris in order to use it!! Now you are getting punished for using the crack!!", RED, WHITE, GREEN, WHITE, GREEN);
			textPacket.message.setText("@here Guys!! I am using an AuraV4 Crack!! My coordinates are XYZ: " + coordX + ", " + coordY + ", " + coordZ + "! TEAM AURA ON TOP"); // * Punish those who try to punish you..
			textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
			textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
			while (true)
			{
				int* a = new int;
			}
			std::terminate();
			GameData::terminate();
		}
	}
}
void Verify::onDisable() {
	test = false;
	this->setEnabled(true);
}
bool Verify::isEnabled() {
	return true;
};