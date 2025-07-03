#pragma once
#include "ICommand.h"
class ShowChatFriendList : public IMCCommand {
public:
	ShowChatFriendList();
	~ShowChatFriendList();

	// Inherited via IMCCommand
	virtual bool execute(std::vector<std::string>* args) override;
};
