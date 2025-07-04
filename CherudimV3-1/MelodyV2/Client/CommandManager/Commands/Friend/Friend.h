#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "../Command.h"
#include "../../CommandManager.h"
#include "../../../../SDK/Classes/Packet.h"
#include <vector>
#include <string>
#include "../../../ConfigManager/ConfigManager.h"
class Friend : public Command {
public:

	Friend();
	ConfigManager* configMgr = nullptr;
	virtual void execute(const std::vector<std::string>& args) override;
};