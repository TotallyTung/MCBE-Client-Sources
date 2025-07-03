#pragma once
#include <vector>
#include <string>
#include <algorithm>

class Command {
public:
	std::string name;
	std::string ussage;
	std::string description;
	std::vector<std::string> aliases;

	Command(std::string commandName, std::string descr, std::vector<std::string> alias, std::string Usage) {
		this->name = commandName;
		this->description = descr;
		this->aliases = alias;
		this->ussage = Usage;

		std::transform(commandName.begin(), commandName.end(), commandName.begin(), ::tolower);
		this->aliases.push_back(commandName);
	}

	virtual void execute(const std::vector<std::string>& args) {};
};