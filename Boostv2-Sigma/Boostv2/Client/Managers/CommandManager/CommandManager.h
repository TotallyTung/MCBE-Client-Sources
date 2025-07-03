#pragma once
#include "Commands/CommandBase.h"

#include "Commands/BindCommand.h"
#include "Commands/ConfigCommand.h"
#include "Commands/EjectCommand.h"
#include "Commands/UnbindCommand.h"

class CommandManager {
public:
	static inline std::vector<CommandBase*> commandList;
	static inline char prefix = '.';

	static void init();
	static void shutdown();
	static void execute(const std::string& message);
};