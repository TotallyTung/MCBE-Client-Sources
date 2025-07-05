#include "PasswordCommand.h"
#include "../../Module/ModuleManager.h"

#ifdef _BETA
PasswordCommand::PasswordCommand() : IMCCommand("pw", "Verification step part 2", "<password>") {
	registerAlias("v");
}

PasswordCommand::~PasswordCommand() {
}

inline uint32_t hashMaker(const std::string& str) { // very shitty hasher, but it does the job :P
	uint32_t hash = 0x811c9dc5;
	uint32_t prime = 0x1000193;

	for (int i = 0; i < str.size(); ++i) {
		uint8_t value = str[i];
		hash = hash ^ value;
		hash *= prime;
	}

	return hash;
}

bool PasswordCommand::execute(std::vector<std::string>* args) {
	assertTrue(g_Data.getLocalPlayer() != nullptr);
	if (hasChecked) {
		clientMessageF("You have already verified!");
		return true;
	}

	std::string passwordGet = args->at(1);
	uint32_t passHashed = hashMaker(passwordGet);

	auto verMod = moduleMgr->getModule<Verification>();
	int IGNValue = (verMod->userIs) - 1;

	if (passHashed == matchPW[IGNValue]) {
		clientMessageF("Welcome, authorized user. Movement controls released.");
		hasChecked = true;
		verMod->mustLock = false;
		return true;
	} else if (tries > 0) {
		clientMessageF("Wrong password, %i tries left", tries);
		tries--;
	} else {
		clientMessageF("Password entered too many times!");
		GameData::terminate();
	}
	/*
	*/
}
#endif