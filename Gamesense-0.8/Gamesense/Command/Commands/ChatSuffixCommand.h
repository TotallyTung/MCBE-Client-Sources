#pragma once
#include <sstream>

#include "ICommand.h"

class ChatSuffixCommand : public IMCCommand {
public:
	ChatSuffixCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
#pragma once
