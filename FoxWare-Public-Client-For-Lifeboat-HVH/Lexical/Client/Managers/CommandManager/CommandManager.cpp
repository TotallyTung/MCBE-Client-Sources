#include "CommandManager.h"
#include "../../Client.h"
#include <sstream>

void CommandManager::init() {
	commandList.push_back(new BindCommand());
	commandList.push_back(new ConfigCommand());
	commandList.push_back(new EjectCommand());
	commandList.push_back(new UnbindCommand());
}

void CommandManager::shutdown() {
	for (auto& command : commandList) {
		delete command;
		command = nullptr;
	}
	commandList.clear();
}

void CommandManager::execute(const std::string& message) {
	std::string currentMessage = message;
	currentMessage.erase(currentMessage.begin());

	std::vector<std::string> args;
	std::stringstream ss(currentMessage);
	std::string word;

	while (ss >> word) {
		std::transform(word.begin(), word.end(), word.begin(), ::tolower);
		args.push_back(word);
	}

	if (args.empty())
		return;

	std::string cmd = args[0];
	for (auto& command : commandList) {
		std::vector<std::string> checkList;
		checkList.push_back(command->name);
		for (const std::string& alias : command->aliases)
			checkList.push_back(alias);

		for (std::string check : checkList) {
			std::transform(check.begin(), check.end(), check.begin(), ::tolower);
			if (cmd == check) {
				if (!command->execute(args)) {
					Client::DisplayClientMessage("Failed on execute command '%s%s%s'", MCTF::GRAY, message.c_str(), MCTF::WHITE);
					Client::DisplayClientMessage("%sUsage: %s%c%s %s", MCTF::BOLD, MCTF::RESET, prefix, command->name.c_str(), command->usage.c_str());
				}

				return;
			}
		}
	}

	Client::DisplayClientMessage("Couldn't find command %s%s%s!", MCTF::GRAY, cmd.c_str(), MCTF::WHITE);
}