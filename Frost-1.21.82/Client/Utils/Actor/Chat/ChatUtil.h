#pragma once

class ChatUtil
{
public:
	static void displayClientMessage(const std::string& msg)
	{
		Player* mPlayer = InstanceManager::get<ClientInstance>() == nullptr ? nullptr : InstanceManager::get<ClientInstance>()->getLocalPlayer();
		if (!mPlayer) return;

		InstanceManager::get<ClientInstance>()->getGuiData()->displayClientMessage(msg);
	}

	static void sendMessage(std::string message)
	{
		Player* mPlayer = InstanceManager::get<ClientInstance>() == nullptr ? nullptr : InstanceManager::get<ClientInstance>()->getLocalPlayer();
		if (!mPlayer) return;
		displayClientMessage(Util::combine(TextFormat::getColorCode(TextFormat::Color::GRAY), "[", TextFormat::getColorCode(TextFormat::Color::AQUA), "Frost", TextFormat::getColorCode(TextFormat::Color::GRAY), "] ", TextFormat::getStyleCode(TextFormat::Style::RESET), message));
	}

	static void sendNormalMessage(std::string message)
	{
		Player* mPlayer = InstanceManager::get<ClientInstance>() == nullptr ? nullptr : InstanceManager::get<ClientInstance>()->getLocalPlayer();
		if (!mPlayer) return;
		displayClientMessage(message);
	}
};
