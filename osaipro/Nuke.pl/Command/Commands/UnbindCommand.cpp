#include "UnbindCommand.h"

#include "../../Module/ModuleManager.h"

UnbindCommand::UnbindCommand() : IMCCommand("unbind", "unbind modules", "<module | all>") {
}

UnbindCommand::~UnbindCommand() {
}

bool UnbindCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() >= 2);
	std::string moduleName = args->at(1);

	assertTrue(moduleName.size() > 0);

	if (moduleName == "all") {
		if (args->size() >= 3 && args->at(2) == "force") {
			std::vector<std::shared_ptr<IModule>>* modules = moduleMgr->getModuleList();
			for (auto it = modules->begin(); it != modules->end(); ++it) {
				auto mod = *it;
				mod->setKeybind(0x0);
			}
			clientMessageF(u8"%s%sUnbound all modules!", Utils::getCommandClientName(), YELLOW);
		} else {
			clientMessageF(u8"%s%sAre you sure? This will unbind %sALL%s%s modules!", Utils::getCommandClientName(), RED, BOLD, RESET, RED);
			clientMessageF(u8"%s%sUse %s.unbind all force%s to unbind all modules", Utils::getCommandClientName(), RED, WHITE, RED);
		}
		return true;
	}

	auto modOpt = moduleMgr->getModuleByName(moduleName);
	if (!modOpt.has_value()) {
		clientMessageF(u8"%s%sCould not find module with name: %s", Utils::getCommandClientName(), RED, moduleName.c_str());
		return true;
	} 

	auto mod = modOpt.value();
	mod->setKeybind(0x0);
	clientMessageF(u8"%s%sSuccessfully unbound %s!", Utils::getCommandClientName(), GREEN, mod->getModuleName());
	return true;
}
