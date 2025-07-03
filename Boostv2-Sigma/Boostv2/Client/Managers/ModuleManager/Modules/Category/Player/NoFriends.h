#pragma once
#include "../../ModuleBase/Module.h"
#include <unordered_map>
#include <string>

class NoFriends : public Module {
private:
	std::unordered_map<std::string, bool> originalFriends;
public:
	NoFriends();
	void onEnable() override;
	void onDisable() override;
};