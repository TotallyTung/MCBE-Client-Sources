#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "../Command.h"
#include "../../CommandManager.h"
#include "../../../../SDK/Classes/Packet.h"
#include <vector>
#include <string>
class Test : public Command {
public:

	Test();

	virtual void execute(const std::vector<std::string>& args) override;
};