#pragma once

class ChatUtil
{
public:
	static void sendMessage(std::string message)
	{
		//Game::actorplayer->DisplayClientMessage(message.c_str());
	}

	static void sendPlainMessage(std::string message, bool displayArrow = true)
	{
		std::string arrow = displayArrow ? "» " : "";
		//Game::actorplayer->DisplayClientMessage(combine(LIGHT_PURPLE, arrow, RESET, message).c_str());
	}
};
