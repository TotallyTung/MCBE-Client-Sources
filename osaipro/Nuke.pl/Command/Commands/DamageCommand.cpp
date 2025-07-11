#include "DamageCommand.h"

#include "../../Module/ModuleManager.h"

DamageCommand::DamageCommand() : IMCCommand("damage", "Damage yourself (relies on falldamage)", "<amount>") {
	registerAlias("dmg");
}

DamageCommand::~DamageCommand() {
}

bool DamageCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer() != nullptr);
	float amount = assertFloat(args->at(1));
	if (amount < 1.f) {
		clientMessageF(u8"%s%sPlease put a number bigger than 0", Utils::getCommandClientName(), RED);
		return true;
	}

	return true;
}
