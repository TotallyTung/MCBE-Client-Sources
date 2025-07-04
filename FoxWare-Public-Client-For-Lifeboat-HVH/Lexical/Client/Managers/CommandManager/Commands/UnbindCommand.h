#pragma once
#include "CommandBase.h"

class UnbindCommand : public CommandBase {
public:
	UnbindCommand();
	bool execute(const std::vector<std::string>& args) override;
};