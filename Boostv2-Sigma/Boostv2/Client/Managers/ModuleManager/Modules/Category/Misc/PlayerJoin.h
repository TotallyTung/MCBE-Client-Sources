#pragma once
#include "../../ModuleBase/Module.h"

class PlayerJoin : public Module {
private:


public:
	PlayerJoin();

	void onNormalTick(LocalPlayer* localPlayer);

	std::vector<std::string> getCurrentPlayerList();


};