#pragma once
#include "CommandBase.h"

class ConfigCommand : public CommandBase {
public:
	ConfigCommand();
	bool execute(const std::vector<std::string>& args) override;
};