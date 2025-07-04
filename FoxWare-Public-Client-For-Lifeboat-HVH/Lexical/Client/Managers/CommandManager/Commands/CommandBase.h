#pragma once
#include <string>
#include <vector>

class CommandBase {
public:
	std::string name;
	std::string description;
	std::string usage;
	std::vector<std::string> aliases;
public:
	CommandBase(std::string _name, std::string _description, std::string _usage, std::vector<std::string> _aliases = {});
	virtual bool execute(const std::vector<std::string>& args);
};