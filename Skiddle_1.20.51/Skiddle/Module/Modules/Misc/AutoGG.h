#pragma once

class AutoGG : public Module
{
private:
	int mode = 0;

public:
	AutoGG(int keybind = Keyboard::NONE, bool enabled = false) :
		Module("Auto GG", "Misc", "Send kill messages", keybind, enabled)
	{
		registerEnumSetting("Server", "The mode of server", { "Hive" }, &mode);
	}


	void onEvent(UpdateEvent* event) {
		auto player = Game::GetLocalPlayer();
		if (player == nullptr) return;

		switch (mode) {
		case 0: // Hive
			if (Game::sendGG) {
				TextPacket pkt;
				pkt.messageType = 1;
				pkt.translationNeeded = false;

				TextHolder msg((std::string)"GG");
				pkt.message = msg;
				Game::GetInstance()->getPacketSender()->sendToServer(&pkt);
				Game::sendGG = false;
			}
			break;
		}
	}
};