#include "Chat.h"

Chat::Chat() : IModule(0, Category::MISC, "Adds the client suffix") {

}

Chat::~Chat() {
}

const char* Chat::getModuleName() {
	return "Chat";
}

void Chat::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_TextPacket>()) {
		C_TextPacket* funy = reinterpret_cast<C_TextPacket*>(packet);
		std::string Sentence;
		std::string end;
		std::string green;
		green = ">";
		end = " UwU";
		Sentence = green + funy->message.getText() + end;
		funy->message.resetWithoutDelete();
		funy->message = Sentence;
	}
}