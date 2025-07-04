#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "../Command.h"
#include "../../CommandManager.h"
#include "../../../../SDK/Classes/Packet.h"
#include <vector>
#include <string>
class Baritone : public Command {
public:

	Baritone();

	virtual void execute(const std::vector<std::string>& args) override;
};