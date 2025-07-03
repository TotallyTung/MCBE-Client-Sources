#include "BindCommand.h"
#include "../../../Client.h"

BindCommand::BindCommand() : CommandBase("bind", "Binds modules to specific keys", "<module> <key>", { "b" }) {
}

bool BindCommand::execute(const std::vector<std::string>& args) {
	if (args.size() < 3)
		return false;

	std::string moduleNeedToFind = args[1];
	Module* currentModule = nullptr;

	for (auto& mod : ModuleManager::moduleList) {
		std::string moduleName = mod->getModuleName();
		std::transform(moduleName.begin(), moduleName.end(), moduleName.begin(), ::tolower);
		if (moduleName == moduleNeedToFind) {
			currentModule = mod;
			break;
		}
	}

	if (currentModule == nullptr) {
		Client::DisplayClientMessage("Couldn't find module with name: %s%s", MCTF::GRAY, moduleNeedToFind.c_str());
		return true;
	}

	std::string key = args[2];
	if (key.size() > 1) {
		if (key == "none") {
			currentModule->setKeybind(0x0);
			Client::DisplayClientMessage("Successfully unbound %s%s", MCTF::GRAY, currentModule->getModuleName().c_str());
			return true;
		}

		const char* needle = key.c_str();
		for (int i = 0; i < 190; i++) {
			const char* haystack = KeyNames[i];
			size_t len = strlen(needle) + 1;
			char* haystackLowercase = new char[len];
			for (int i = 0; i < len; i++)
				haystackLowercase[i] = tolower(haystack[i]);

			if (strcmp(needle, haystackLowercase) == 0) {
				currentModule->setKeybind(i);
				Client::DisplayClientMessage("The keybind of %s%s%s is now '%s%s%s'", MCTF::GRAY, currentModule->getModuleName().c_str(), MCTF::RESET, MCTF::GRAY, haystack, MCTF::RESET);
				delete[] haystackLowercase;
				return true;
			}
			delete[] haystackLowercase;
		}
		Client::DisplayClientMessage("%sInvalid key!", MCTF::RED);
		return true;
	}
	int keyCode = (int)key[0];
	keyCode = ::toupper(keyCode);
	if (keyCode > 0 && keyCode < 192) {
		currentModule->setKeybind(keyCode);
		Client::DisplayClientMessage("The keybind of %s%s%s is now '%s%c%s'", MCTF::GRAY, currentModule->getModuleName().c_str(), MCTF::RESET, MCTF::GRAY, (char)keyCode, MCTF::RESET);
		return true;
	}
	else {
		Client::DisplayClientMessage("%sInvalid key!", MCTF::RED);
		return true;
	}

	return false;
}