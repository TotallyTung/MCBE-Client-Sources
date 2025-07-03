#include "PanicCommand.h"

#include "../../Module/ModuleManager.h"

PanicCommand::PanicCommand() : IMCCommand("panic", "Disables all Modules", "") {
}

PanicCommand::~PanicCommand() {
}

bool PanicCommand::execute(std::vector<std::string>* args) {
	std::vector<std::shared_ptr<IModule>> * modules = moduleMgr->getModuleList();

	for (auto it = modules->begin(); it != modules->end(); ++it) {
		auto mod = *it;
		mod->setEnabled(false);
	}
	clientMessageF(u8"%s%sDisabled all Modules!", Utils::getCommandClientName(), YELLOW);
	return true;
}
