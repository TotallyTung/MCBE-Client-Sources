#include "AutoLog.h"
AutoLog::AutoLog() : IModule(0, Category::PLAYER, "EZ LOG") {
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
	LogType.addEntry("Packet", 0);
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
void AutoLog::onTick(C_GameMode* gm) {
	auto plr = g_Data.getLocalPlayer();
	int healthy = g_Data.getLocalPlayer()->getHealth();
	if (wait_interval >= 0.1f) {
		wait_interval -= 0.1f;
		return;
	}
	switch (ByeByeMessage.getSelectedValue()) {
	case 1: {
		text = "ByeBye!!";
	}
	case 2: {
		if (healthy < health && healthy > health - 2) text = u8"ByeBye!!!";
		if (healthy < health && healthy < health - 2) text = u8"Goodbye!!";
	}
	}
	if (healthy < health) {
		bool OnArmor = false;
		if (!OnArmor) {
			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = plr->getArmor(i);
				if (stack->isValid()) OnArmor = true;
			}
		}
		if (healthy < log_health && plr->isAlive() && OnArmor) {
			if (LogType.getSelectedValue() == 0) {
				if (ByeByeMessage.getSelectedValue() != 0) Utils::sendChat(text);
				for (int pp = 0; pp < 1000; pp++) {
					C_MovePlayerPacket movePacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
				}
			};
			}
		}
	}


void AutoLog::onJoiningServer() {
	wait_interval = interval;
}