#include "UnbindCommand.h"
#include "../../../Client.h"

UnbindCommand::UnbindCommand() : CommandBase("unbind", "unbind modules", "<module | all>") {
}

bool UnbindCommand::execute(const std::vector<std::string>& args) {
	if (args.size() < 2)
		return false;

	std::string moduleNeedToFind = args[1];
	if (moduleNeedToFind == "all") {
		for (auto& mod : ModuleManager::moduleList) {
			mod->setKeybind(0x0);
		}
		Client::DisplayClientMessage("Successfully unbound %sall modules%s!", MCTF::GRAY, MCTF::WHITE);
		return true;
	}

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

	currentModule->setKeybind(0x0);
	Client::DisplayClientMessage("Successfully unbound %s%s", MCTF::GRAY, currentModule->getModuleName().c_str());
	return true;
}