#pragma once
#include "CommandBase.h"

class BindCommand : public CommandBase {
public:
	BindCommand();
	bool execute(const std::vector<std::string>& args) override;
};