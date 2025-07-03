#pragma once
#include "CommandBase.h"

class EjectCommand : public CommandBase {
public:
	EjectCommand();
	bool execute(const std::vector<std::string>& args) override;
};