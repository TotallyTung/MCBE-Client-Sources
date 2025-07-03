#include "XUIDCommand.h"

XUIDCommand::XUIDCommand() : IMCCommand("xuid", "Tells your xuid", "") {
	registerAlias("uid");
}

bool XUIDCommand::execute(std::vector<std::string>* args) {
	auto XUID = g_Data.getLocalPlayer()->getUserId();
	clientMessageF("[Gamesense] %sXUID: %", GREEN, XUID);
	return true;
}