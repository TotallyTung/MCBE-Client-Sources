#pragma once
#include "../../ModuleBase/Module.h"
#include <unordered_map>
#include <string>

class MidClick : public Module {
private:
	std::unordered_map<std::string, bool> friends;
public:
	MidClick();
	void onNormalTick(LocalPlayer* localPlayer) override;
	bool isFriend(const std::string& playerName);
	std::unordered_map<std::string, bool>& getFriends();
};