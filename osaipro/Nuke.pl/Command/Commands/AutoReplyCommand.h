#pragma once
#include "ICommand.h"
#include "../../Module/ModuleManager.h"
class AutoReplyCommand : public IMCCommand {
public:
	AutoReplyCommand();
	~AutoReplyCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
