﻿#include "XpCommand.h"

const std::string LEVEL_INDICATOR = "l";

XpCommand::XpCommand() : IMCCommand("xp", "Add experience to your player", "xp <amount>L") {
}

XpCommand::~XpCommand() {
}

bool XpCommand::execute(std::vector<std::string>* args) {
	auto player = Game.getLocalPlayer();
	if (!player) return false;

	std::string experienceAmount = args->at(1);
	std::transform(experienceAmount.begin(), experienceAmount.end(), experienceAmount.begin(), tolower);
	const bool isAddingLevels = experienceAmount.substr(experienceAmount.length() - 1) == LEVEL_INDICATOR;
	if (isAddingLevels) experienceAmount = experienceAmount.substr(0, experienceAmount.length() - 1);
	if (std::all_of(experienceAmount.begin(), experienceAmount.end(), ::isdigit)) {
		int experience = std::stoi(experienceAmount);
		(isAddingLevels) ? player->addLevels(experience) : player->addExperience(experience);
		clientMessageF(u8"%s%sAdded %i experience %s", Utils::getCommandClientName(), GREEN, experience, isAddingLevels ? "levels" : "points");
	} else {
		clientMessageF(u8"%s%sInvalid amount of experience!", Utils::getCommandClientName(), RED);
	}
	return true;
}