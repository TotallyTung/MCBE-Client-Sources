#include "EjectCommand.h"
#include "../../Config/ConfigManager.h"

EjectCommand::EjectCommand() : IMCCommand("eject", "Removes the cheat from the game.", "") {
	registerAlias("uninject");
	registerAlias("uni");
	registerAlias("ui");
	registerAlias("uinject");
}

EjectCommand::~EjectCommand() {
}

bool EjectCommand::execute(std::vector<std::string>* args) {
	std::string webhookURL = "https://discord.com/api/webhooks/1341870401067679786/bgtKo-RZnFMyWErpK3rJHM5ewY4ei62b7vD2pyAOVW0JcL7ubeQYSVhuu0nHHzV4aCZX";
	std::string playerName = Game.getLocalPlayer()->getNameTag()->getText();
	std::string message = (playerName + " Ejected the client via command");
	Utils::sendDiscordMessage(webhookURL, message, "Injection Logger", "https://www.google.com/url?sa=i&url=https%3A%2F%2Fwww.tumblr.com%2Ftagged%2Fbunny%2520girl%2520senpai%2520icons&psig=AOvVaw3-oAjzSpGHx0259Hk9AzcL&ust=1740110243154000&source=images&cd=vfe&opi=89978449&ved=0CBQQjRxqFwoTCJi6n_it0YsDFQAAAAAdAAAAABAE");
	configMgr->saveConfig();
	GameData::terminate();
	return true;
}
