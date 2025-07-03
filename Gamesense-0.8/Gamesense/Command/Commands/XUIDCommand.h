#pragma once
#include "ICommand.h"

class XUIDCommand : public IMCCommand {
public:
	virtual bool execute(std::vector<std::string>* args) override;
	XUIDCommand();
};
