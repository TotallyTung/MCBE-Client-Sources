#pragma once
#include "Player.h"

class LocalPlayer : public Player {
public:
	void displayClientMessage(const std::string& message) {
		MemoryUtil::CallVFunc<213, void, const std::string&>(this, message);
	}
	void playEmote(std::string const& sex) {
		return MemoryUtil::CallVFunc<222, void, std::string const&>(this, sex);
	}
	void addExperience(int sex) {
		return MemoryUtil::CallVFunc<231, void, int*>(this, &sex);
	}
};
