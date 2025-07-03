#include "../ModuleManager.h"

#include "ChatSuffix.h"

#include "../../Utils/Utils.h"
std::string Sentence;
std::string release;
std::string Suffix = Utils::getClientName();
ChatSuffix::ChatSuffix() : IModule(0, Category::MISC, "Adds the client suffix") {
	registerBoolSetting("Bypass1", &bypass1, bypass1);
	registerIntSetting("Bypass Length 1", &bypass1length, bypass1length, 1, 40);
	registerBoolSetting("Bypass2", &bypass2, bypass2);
	registerIntSetting("Bypass Length 2", &bypass2length, bypass2length, 1, 40);
	registerBoolSetting("Suffix", &suffix, suffix);
	registerBoolSetting("Green", &green, green);
	registerBoolSetting("Reset", &reset, reset);
}

ChatSuffix::~ChatSuffix() {
}

const char* ChatSuffix::getModuleName() {
	return "Chat+";
}

void ChatSuffix::onTick(GameMode* gm) {
	if (reset) {
		green = true;
		bypass1 = true;
		bypass2 = true;
		suffix = true;
		bypass1length = 4;
		bypass2length = 9;
		reset = false;
	}
}

void ChatSuffix::onSendPacket(Packet* packet) {
	if (packet->isInstanceOf<C_TextPacket>()) {
		C_TextPacket* funy = reinterpret_cast<C_TextPacket*>(packet);

		std::string start;
		std::string end;
		if(green == true) {
			start += u8"˜a> ";
		};
		if (bypass1 == true) {
			start += Utils::randomString(bypass1length) + " | ";
		};
		if (suffix == true && bypass2 == false) {
			end += " | " + Suffix;
		}
		if (bypass2 == true && suffix == false) {
			end += " | " + Utils::randomString(bypass2length);
		}
		if (bypass2 == true && suffix == true) {
			end += " | " + Suffix + " | " + Utils::randomString(bypass2length);
		}
		Sentence = start + funy->message.getText() + end;
		funy->message.resetWithoutDelete();
		funy->message = Sentence;
	}
}