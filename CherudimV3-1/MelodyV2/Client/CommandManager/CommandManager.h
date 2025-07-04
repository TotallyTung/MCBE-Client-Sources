#pragma once
#include "Commands/Command.h"
#include "Commands/Baritone/Baraton.h"
#include "Commands/Save/SaveCommand.h"
#include "Commands/ItemId/ItemId.h"
#include "Commands/Bind/BindCommand.h"
#include "Commands/IRC/IRC.h"
#include "Commands/Test/Test.h"
#include "Commands/Friend/Friend.h"
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