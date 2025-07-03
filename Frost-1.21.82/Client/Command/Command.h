#pragma once

class Command
{
public:
	Command(std::string commandName, std::string descr, std::vector<std::string> aliases = std::vector<std::string>())
		: mName(commandName), mDescr(descr), mAliases(aliases)
	{}

	virtual bool execute(std::vector<std::string> cmd) { return false; }

	std::string mName = "";
	std::string mDescr = "";
	std::vector<std::string> mAliases;
};
