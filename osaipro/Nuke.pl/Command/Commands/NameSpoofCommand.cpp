#include "NameSpoofCommand.h"

NameSpoofCommand::NameSpoofCommand() : IMCCommand("namespoof", "Spoof your name on realms/bds", "<name/reset> <name>") {
	registerAlias("ns");
	registerAlias("fakename");
	registerAlias("fn");
}

NameSpoofCommand::~NameSpoofCommand() {
}

bool NameSpoofCommand::execute(std::vector<std::string>* args) {
	if (args->at(1) == "name" && args->size() > 2) {
		std::ostringstream os;
		for (int i = 2; i < args->size(); i++) {
			if (i > 2)
				os << " ";
			os << args->at(i);
		}
		TextHolder* name = new TextHolder(os.str());
		Game.setFakeName(name);
		clientMessageF(u8"%s%sSet fakename to %s%s%s, please reconnect!", Utils::getCommandClientName(), GREEN, GRAY, name->getText(), GREEN);
		return true;
	} else if (args->at(1) == "reset") {
		Game.setFakeName(NULL);
		clientMessageF(u8"%s%sReset fakename!", Utils::getCommandClientName(), GREEN);
		return true;
	}
	return false;
}
