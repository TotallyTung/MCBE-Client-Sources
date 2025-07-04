#include "ChatSuffix.h"

ChatSuffix::ChatSuffix() : IModule(0, Category::OTHER, "Adds the client suffix") {
}

ChatSuffix::~ChatSuffix() {
}

const char* ChatSuffix::getModuleName() {
	return "Chat+";
}

void ChatSuffix::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_TextPacket>()) {
	//	C_TextPacket* funy = reinterpret_cast<C_TextPacket*>(packet);
	//	std::string Sentence;
	//	std::string end;

	//	end = " | Nuke";
	//	Sentence = funy->message.getText() + end;
	//	funy->message.resetWithoutDelete();
	//	funy->message = Sentence;
		C_TextPacket* textPacket = reinterpret_cast<C_TextPacket*>(packet);
		textPacket->message = std::string("> ") + textPacket->message.getText() + std::string(" | AuraV4");
	}
}

