#include "EjectCommand.h"
#include "../../../Client.h"

EjectCommand::EjectCommand() : CommandBase("eject", "Removes the cheat from the game.", "", { "uninject" }) {
}

bool EjectCommand::execute(const std::vector<std::string>& args) {
	Client::shutdown();
	return true;
}