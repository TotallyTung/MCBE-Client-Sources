#pragma once

class ChatUtils
{
public:

	static void sendMessage(std::string message)
	{
		Address::getLocalPlayer()->displayClientMessage(Utils::combine(BOLD, BLACK, "[", LIGHT_PURPLE, "Floence", BLACK, "] ", RESET, message));
	}

	static void sendDisablerMessage(std::string message)
	{
		Address::getLocalPlayer()->displayClientMessage(Utils::combine(BOLD, BLACK, "[", LIGHT_PURPLE, "Floence", BLACK, "] ", RESET, message));
	}

	static void sendCustomMessage(std::string message, std::string title)
	{
		Address::getLocalPlayer()->displayClientMessage(Utils::combine(BOLD, BLACK, "[", LIGHT_PURPLE, "Floence", BLACK, "] ", RESET, message));
	}

	static void sendNormalMessage(std::string message)
	{
		Address::getLocalPlayer()->displayClientMessage(message);
	}
};
