#pragma once
#include <map>

#include "ICommand.h"

class CombatInfoCommand : public IMCCommand {
public:
	CombatInfoCommand();
	~CombatInfoCommand();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
#pragma once
