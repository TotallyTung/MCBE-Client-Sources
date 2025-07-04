#include "ChatSuffix.h"
bool green = true;
int mode2 = 0;
ChatSuffix::ChatSuffix() : Module("ChatSuffix", "Add a suffix to your chat messages", Category::MISC) {
	addEnumSetting("Mode", "NULL", { "BoostClient", "Boost", "NukeV6", "Luna", "SakuraSense" }, &mode2);
}

void ChatSuffix::onSendPacket(Packet* packet, bool& shouldCancel) {
	if (packet->getId() == PacketID::Text) {
		TextPacket* textPacket = reinterpret_cast<TextPacket*>(packet);
		std::string message = textPacket->message;
		if (green)
			textPacket->message = std::string(">" + message);
		switch (mode2) {
		case 0:
			textPacket->message = std::string(message + std::string(" | Boost Client"));
			break;
		case 1:
			textPacket->message = std::string(message + std::string(" | Boost"));
			break;
		case 2:
			textPacket->message = std::string(message + std::string(" | NukeV6"));
			break;
		case 3:
			textPacket->message = std::string(message + std::string(" | Luna"));
			break;
		case 4:
			textPacket->message = std::string(message + std::string(" | SakuraSense"));
			break;
		}
	}
}
