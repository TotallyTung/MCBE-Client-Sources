#pragma once
#include <sstream>

#include "ICommand.h"

class KitName : public IMCCommand {
public:
	KitName();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
#pragma once