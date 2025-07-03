#include "AutoLog.h"
AutoLog::AutoLog() : IModule(0, Category::PLAYER, "Spams so many packets server kicks you") {
	registerIntSetting("MsgHealth", &health, 10, 1, 20);
	registerIntSetting("LogHealth", &log_health, 5, 1, 20);
	registerFloatSetting("interval", &interval, interval, 0.f, 30.f);
	registerEnumSetting("Text Modes", &ByeByeMessage, 0);
	registerEnumSetting("Log Modes", &LogType, 0);
	ByeByeMessage = SettingEnum(this);
	ByeByeMessage.addEntry("None", 0);
	ByeByeMessage.addEntry("ByeBye!!", 1);
	ByeByeMessage.addEntry("rabbit", 2);
	LogType = SettingEnum(this);
	LogType.addEntry("None", 0);
	LogType.addEntry("Packet", 1);
	LogType.addEntry("Command", 2);
}
AutoLog::~AutoLog() {
}

const char* AutoLog::getModuleName() {
	return ("AutoLog");
}
const char* AutoLog::getModName() {
	if (wait_interval >= 0.1f) return std::to_string(wait_interval).c_str();
	if (wait_interval <= 0.f) return std::to_string(log_health).c_str();
}
std::string text;
void AutoLog::onTick(GameMode* gm) {
	auto plr = Game.getLocalPlayer();
	int healthy = Game.getLocalPlayer()->getHealth();
	if (wait_interval >= 0.1f) {
		wait_interval -= 0.1f;
		return;
	}
	switch (ByeByeMessage.getSelectedValue()) {
	case 1: {
		text = "ByeBye!!";
	}
	case 2: {
		if (healthy < health && healthy > health - 2) text = u8"あぶねぇあぶねぇ、死にかけたぜw";
		if (healthy < health && healthy < health - 2) text = u8"あぶなすぎあぶなすぎ！！ガチで死にかけたぜ...";
	}
	}
	if (healthy < health) {
		bool OnArmor = false;
		if (!OnArmor) {
			for (int i = 0; i < 4; i++) {
				ItemStack* stack = plr->getArmor(i);
				if (stack->isValid()) OnArmor = true;
			}
		}
		if (healthy < log_health && plr->isAlive() && OnArmor) {
			if (LogType.getSelectedValue() == 1) {
				if (ByeByeMessage.getSelectedValue() != 0) Utils::sendChat(text);
				for (int pp = 0; pp < 1000; pp++) {
					C_MovePlayerPacket movePacket(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());
					Game.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
				}
			};
			if (LogType.getSelectedValue() == 2) moduleMgr->getModule<hub>()->setEnabled(true);
		}
	}
}

void AutoLog::onJoiningServer() {
	wait_interval = interval;
}