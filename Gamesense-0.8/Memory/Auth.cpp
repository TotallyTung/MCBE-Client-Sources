#include "Auth.h"
#include "GameData.h"
#include "../Gamesense/Module/ModuleManager.h"
#include <string>
#include <algorithm>
using namespace std;
void Auth::runAuth() {
    std::string authorizedUsers[] = {
	    "MerPlayzMC",
		"MerciGGs",
		"CPE Is Cancer",
		"Afar Arkaris",
		"Irvin289139",
		"Jepium",
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
		"toastedbuter773",
		"GhOsT37664",
		"Iammtrashhh",
		"Hades on top796",
		"Zh1n1anQwQ",
		"IllLilMartin1",
		"x17r",
		"bendzone",
		"blad blad bung",
		"supsoup86",
		"Nova dk mod5116",
		"fortsimp6402",
		"mrmadman2833",
		"s9177",
		"Tigerpng",
		"LeftAstro7915",
		"zqe2"
	};
    if (g_Data.getLocalPlayer() != nullptr) {
        std::string playerID = g_Data.getLocalPlayer()->getNameTag()->getText();
        if (std::find(std::begin(authorizedUsers), std::end(authorizedUsers), playerID) != std::end(authorizedUsers)) {
            //auto notification = g_Data.addNotification("Gamesense:", "Welcome to Gamesense v" + moduleMgr->getModule<Interface>()->versionStr + " " + playerID);
            //notification->duration = 10;
        }
        else {
            C_TextPacket textPacket;
            vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
            string coords =
                std::to_string((int)floorf(currPos->x)) + ", " +
                std::to_string((int)floorf(currPos->y)) + ", " +
                std::to_string((int)floorf(currPos->z));
            std::string message = "@here i just tried to use a Gamesense Client crack, My coordinates is XYZ: " + coords + "! GET GOOD. GET GAMESENSE";
            textPacket.message.setText(message);
            textPacket.sourceName.setText(g_Data.getLocalPlayer()->getNameTag()->getText());
            textPacket.xboxUserId = std::to_string(g_Data.getLocalPlayer()->getUserId());
            g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
            GameData::terminate();
        }
    }
}