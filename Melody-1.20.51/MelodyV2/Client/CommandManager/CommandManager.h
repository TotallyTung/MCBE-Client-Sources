// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "Commands/Command.h"
#include "Commands/Baritone/Baraton.h"
#include "Commands/Save/SaveCommand.h"
#include "Commands/ItemId/ItemId.h"
#include "Commands/Bind/BindCommand.h"
#include "Commands/IRC/IRCom.h"
#include "Commands/Test/Test.h"
#include "Commands/Friend/Friend.h"
#include "Commands/PlayerList/PlayerList.h"
#include "../../SDK/Classes/Packet.h"
#include <vector>
#include <string>

class CommandManager {
private:
	std::vector<Command*> commandList;
public:

	char prefix = '.';
	CommandManager();
	~CommandManager();
	void onSendTextPacket(TextPacket* packet, bool& shouldCancel);
};