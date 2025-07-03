#pragma once

class AutoQueue : public Module
{
private:
	int mode = 0;
	int game = 0;

public:
	AutoQueue(int keybind = Keyboard::NONE, bool enabled = false) :
		Module("Auto Queue", "Misc", "Send next game", keybind, enabled)
	{
		registerEnumSetting("Mode", "The mode of message", { "Hive" }, &mode);
		registerEnumSetting("Game", "The mode", { "None", "Skywars", "Treasure" }, &game);
	}


	void onEvent(UpdateEvent* event) {
		auto player = Game::GetLocalPlayer();
		if (player == nullptr) return;

		if (Game::sendQueue) {
			CommandRequestPacket pkt;
			switch (game) {
			case 0: // None
				pkt.command.setText("/q");
				break;
			case 1: // Skywars
				pkt.command.setText("/q sky");
				break;
			case 2: // Treasure
				pkt.command.setText("/q tw");
				break;
			}
			Game::GetInstance()->getPacketSender()->sendToServer(&pkt);
			Game::sendQueue = false;
		}
	}
};