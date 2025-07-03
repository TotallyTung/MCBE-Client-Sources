// Created by Tony on 2024-10-10 10:01:54

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
class Bind : public Command {
public:

	Bind();
	ConfigManager* configMgr = nullptr;
	virtual void execute(const std::vector<std::string>& args) override;
};