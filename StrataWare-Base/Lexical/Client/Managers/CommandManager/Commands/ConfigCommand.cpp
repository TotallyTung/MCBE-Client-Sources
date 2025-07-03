#include "ConfigCommand.h"
#include "../../../Client.h"

ConfigCommand::ConfigCommand() : CommandBase("config", "Create/load/save configs", "<load/new/save> <name>", { "c" }) {
}

bool ConfigCommand::execute(const std::vector<std::string>& args) {
	if (args[1] == "load") {
		if (args.size() < 3)
			return false;
		std::string name = args[2];
		ConfigManager::loadConfig(name);
		return true;
	}
	else if (args[1] == "new") {
		if (args.size() < 3)
			return false;
		std::string name = args[2];
		ConfigManager::createNewConfig(name);
		return true;
	}
	else if (args[1] == "save") {
		ConfigManager::saveConfig();
		Client::DisplayClientMessage("Successfully saved config %s%s%s!", MCTF::GRAY, ConfigManager::currentConfig.c_str(), MCTF::WHITE);
		return true;
	}
	return false;
}