#pragma once

class ChatUtil
{
public:

	static void sendMessage(std::string message)
	{
		Game::GetLocalPlayer()->DisplayClientMessage(combine(BOLD, LIGHT_PURPLE, Game::Skiddle::ClientName, " » ", RESET, message).c_str());
		// Game::GetLocalPlayer()->DisplayClientMessage(combine(BOLD, "[", WHITE, Game::Lunacia::ClientName, BOLD, "] ", RESET, message).c_str());
	}

	static void sendPlainMessage(std::string message, bool displayArrow = true)
	{
		std::string arrow = displayArrow ? "» " : "";
		Game::GetLocalPlayer()->DisplayClientMessage(combine(WHITE, arrow, RESET, message).c_str());
	}
};
