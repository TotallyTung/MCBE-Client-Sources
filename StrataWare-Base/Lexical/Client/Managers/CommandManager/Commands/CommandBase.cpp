#include "CommandBase.h"

CommandBase::CommandBase(std::string _name, std::string _description, std::string _usage, std::vector<std::string> _aliases) {
	this->name = _name;
	this->description = _description;
	this->usage = _usage;
	this->aliases = _aliases;
}

bool CommandBase::execute(const std::vector<std::string>& args) {
	return true;
}