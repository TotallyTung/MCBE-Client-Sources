#pragma once
#include "../../ModuleBase/Module.h"

class DamageText : public Module {
public:

private:
	std::vector<Actor*> DamageList;
	int damage = 0;
public:
	DamageText();

	void onNormalTick(LocalPlayer* localPlayer);

};