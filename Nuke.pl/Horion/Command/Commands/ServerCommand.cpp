#include "ServerCommand.h"

ServerCommand::ServerCommand() : IMCCommand("server", "Show information about the server you're playing on", "") {
	registerAlias("ip");
}

ServerCommand::~ServerCommand() {
}

bool ServerCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer() != nullptr);

	if (Game.getRakNetInstance()->isonaServer())
		clientMessageF(u8"%sYou're currently playing on:\nIP: %s\nPort: %s", Utils::getCommandClientName(), Game.getRakNetInstance()->serverIp.getText(), std::to_string(Game.getRakNetInstance()->serverPort).c_str());
	else
		clientMessageF(u8"%s%sYou're not playing on a server.", Utils::getCommandClientName(), RED);
	return true;
}
