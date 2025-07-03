#include "AutoReplyCommand.h"
AutoReplyCommand::AutoReplyCommand() : IMCCommand("AutoReply", "Editing AutoReply Texts (/r AutoReply->Your Message)", "<string>") {
}

AutoReplyCommand::~AutoReplyCommand() {
}

bool AutoReplyCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer() != nullptr);
	auto AutoReplyMod = moduleMgr->getModule<AutoReply>();
	std::ostringstream os;
	for (int i = 1; i < args->size(); i++) {
		if (i > 1)
			os << " ";
		os << args->at(i);
	}
	std::string text = os.str().substr(1);
	AutoReplyMod->Message = text;
	clientMessageF(u8"%s%s%s message set to %s%s%s!", Utils::getCommandClientName(), GREEN, AutoReplyMod->getModuleName(), GRAY, text.c_str(), GREEN);
	return true;
}
