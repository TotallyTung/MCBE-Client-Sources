#include "HideCommand.h"

HideCommand::HideCommand() : IMCCommand("hide", "hide the mod", "") {
}

HideCommand::~HideCommand() {
}

bool HideCommand::execute(std::vector<std::string>* args) {
	GameData::hide();
	if (GameData::shouldHide()) {
		clientMessageF(u8"%s%sHidden.", Utils::getCommandClientName(), GREEN);
	} else {
		clientMessageF(u8"%s%sMod is now visible.", Utils::getCommandClientName(), GREEN);
	}
	return true;
}
