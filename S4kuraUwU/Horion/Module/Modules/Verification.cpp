
#include "../../../Utils/Utils.h"
#include "Verification.h"
#include <string>
#include <stdlib.h>  // to get random integers

Verification::Verification() : IModule(0, Category::MISC, "Checks if you are cool and based enough to use this client") {
}

Verification::~Verification() {
}

const char* Verification::getModuleName() {
	return ("Whitelist");
}	

std::string generateReportID() {
	/* format AABCBBBCDD
	*  A = char A, D, J, W
	*  B = int 0 1 2 5 6 8 9
	*  C = char R, L
	*  D = char T, int 3, 4, 7
	*/
	int ran;
	char idArr[10] = {'X','X','X','X','X','X','X','X','X','X'};  // there's prob a better way, but if it works, it ain't stupid

	const char A[8] = {'A', 'D', 'J', 'W', 'C', 'R', 'Y', 'N'};
	const char B[7] = {'0', '1', '2', '5', '6', '8', '9'};
	const char C[2] = {'R', 'L'};
	const char D[8] = {'T', '3', '4', '7', '5', 'S', 'V', 'J'};

	// generate A
	ran = rand() % 3;
	idArr[0] = A[ran];
	ran = rand() % 3;
	idArr[1] = A[ran];

	// generate B
	ran = rand() % 6;
	idArr[2] = B[ran];

	for (int i = 0; i < 3; i++) {
		ran = rand() % 6;
		idArr[4 + i] = B[ran];
	}

	// generate C
	ran = rand() % 1;
	idArr[3] = C[ran];
	ran = rand() % 1;
	idArr[7] = C[ran];

	// generate D      too lazy to use a loop LOL
	ran = rand() % 3;
	idArr[8] = D[ran];
	ran = rand() % 3;
	idArr[9] = D[ran];

	std::string ID = (idArr);
	return ID;
}

void Verification::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr) return;

	if (test == false) {

		std::string playerIGN = g_Data.getLocalPlayer()->getNameTag()->getText();

		for (auto &checkOne : allowedIGNS) {
			std::size_t strHas = playerIGN.find(checkOne);
			userIs++;
			if (playerIGN == "Nakataba" || playerIGN == "naninaka" || playerIGN == "MoonSakura6738" || playerIGN == "sleepPass244" || playerIGN == "meow Yiu0704" || playerIGN == "MyWaifu 028130" || playerIGN == "Jiasj1ajiX" || playerIGN == "ALuo Cute8662" || playerIGN == "P1ngByp4zzPr0" || playerIGN == "LanGameYT" || playerIGN == "CatBook23520" || playerIGN == "EZ SpammerUwU" || playerIGN == "DsY Emilia" || playerIGN == "hank082896" || playerIGN == "PizzaXD 888" || playerIGN == "UrToast45901" || playerIGN == "You seem soweak" || playerIGN == "KiarawaT") {
				
				mustLock = false;
				return;  // user is offline; let them test in a flatworld XD
			}
			else if (strHas != std::string::npos) {
				passedTest = true;
				test = true;
				mustLock = true;
				break;
			} else
				passedTest = false;
				test = true;
		}

		if (passedTest) {
			clientMessageF("Please verify with .pw in order to unlock movement controls.");
		} else if (passedTest == false) {
			clientMessageF("Sorry, you are unauthorized to use this hack!");
			gm->player->jumpFromGround();
			std::string ID = generateReportID();
			C_TextPacket textPacket;
			textPacket.message.setText("DsY On Top!!!You don't have Whitelist hahaha | Report ID: "+ ID);
			textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
			textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);

			GameData::terminate();
		}
	}

}

void Verification::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr) return;

	if (mustLock) {
		vec3_t yR = vec3_t(floor(g_Data.getLocalPlayer()->getPos()->x), g_Data.getLocalPlayer()->getPos()->y, floor(g_Data.getLocalPlayer()->getPos()->z));
		yR.x += 0.5f;
		yR.x += 0.5f;
		yR.z += 0.5f;
		g_Data.getLocalPlayer()->setPos(yR);
	}

	if (passedTest && mustLock) {
		auto wid = g_Data.getClientInstance()->getGuiData()->windowSize;
		std::string Message = " S4Kura UwU You";
		vec2_t textPos = vec2_t(wid.x / theX - DrawUtils::getTextWidth(&Message, 7.f) / 1.f, wid.y / theY);
		DrawUtils::drawText(textPos, &Message, MC_Color(255, 100, 255), 2.f);
	}
}

void Verification::onDisable() {
	this->setEnabled(true);
}