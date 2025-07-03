#include "GameModeCommand.h"

GameModeCommand::GameModeCommand() : IMCCommand("gamemode", "Changes the local player's gamemode", "<number>") {
	registerAlias("gm");
}

GameModeCommand::~GameModeCommand() {
}

bool GameModeCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer() != nullptr);
	assertTrue(args->size() > 1);
	int gamemode = assertInt(args->at(1));
	if (gamemode >= 0 && gamemode <= 2) {
		Game.getLocalPlayer()->setGameModeType(gamemode);
		clientMessageF(u8"%s%sGameMode changed!", Utils::getCommandClientName(), GREEN);
		return true;
	}

	clientMessageF(u8"%s%sInvalid GameMode!", Utils::getCommandClientName(), RED);
	return true;
}
