#include "AutoLog.h"
bool logaudio;
bool logautolog;
AutoLog::AutoLog() : IModule(0, Category::PLAYER, "Spams so many packets server kicks you") {
	registerIntSetting("Health", &health, health, 1, 20);
	registerEnumSetting("Text Modes", &ByeByeMessage, 0);
	registerEnumSetting("Log Modes", &LogType, 0);
	registerBoolSetting("Log Message", &message, message);
	registerBoolSetting("Log Audio", &logaudio, true);
	registerBoolSetting("Log", &logautolog, true);
	ByeByeMessage = SettingEnum(this);
	ByeByeMessage.addEntry("ByeBye!!", 0);
	ByeByeMessage.addEntry("Good Bye!!", 1);
	ByeByeMessage.addEntry("You Are an idiot!!", 2);
	ByeByeMessage.addEntry(std::string(Utils::getClientName() + " ON TOP").c_str(), 3);
	LogType = SettingEnum(this);
	LogType.addEntry("Packet", 0);
	LogType.addEntry("/hub", 1);
	
}
AutoLog::~AutoLog() {
}

const char* AutoLog::getModuleName() {
	return ("AutoLog");
}
std::string text;
void AutoLog::onTick(GameMode* gm) {
	auto plr = Game.getLocalPlayer();
	if (LogType.getSelectedValue() == 1) {
		message = false;
	}
	switch (ByeByeMessage.getSelectedValue()) {
		case 0: {
			text = "ByeBye!!";
		}
		case 1: {
			text = "Good Bye!!";
		}
		case 2: {
			text = "You Are an idiot!!";
		}
		case 3: {
			text = Utils::getClientName() + " ON TOP";
		}
	}
	int healthy = Game.getLocalPlayer()->getHealth();
	if (healthy < health) {
		if (message == true) {
			Utils::sendChat(text);
		};
		if (logautolog) {
			switch (LogType.getSelectedValue()) {
				case 0: {
					for (int pp = 0; pp < 1000; pp++) {
						C_MovePlayerPacket movePacket(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());
						Game.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
					}
				};
				case 1: {
					Utils::sendCommand("hub");
				};
			};
		};
		if (logaudio) {
			Level* level;
			level->playSound("AutoLogAudio", *Game.getLocalPlayer()->getPos(), 1.0, 1.0);
		};
	}
}