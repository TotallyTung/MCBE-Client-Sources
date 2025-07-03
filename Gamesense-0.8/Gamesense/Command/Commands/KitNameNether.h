#pragma once
#include <sstream>

#include "ICommand.h"

class KitNameNether : public IMCCommand {
public:
	KitNameNether();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
#pragma once