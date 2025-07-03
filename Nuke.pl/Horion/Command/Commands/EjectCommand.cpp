#include "EjectCommand.h"

EjectCommand::EjectCommand() : IMCCommand("eject", "Removes the cheat from the game.", "") {
	registerAlias("uninject");
	registerAlias("uni");
	registerAlias("ui");
	registerAlias("uinject");
}

EjectCommand::~EjectCommand() {
}

bool EjectCommand::execute(std::vector<std::string>* args) {
	GameData::terminate();
	return true;
}
