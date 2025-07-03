#pragma once
#include "../../ModuleBase/Module.h"

class PopCounter : public Module {
public:
	bool totem = false;
	int popcounts = 0;
	std::string names;
	virtual void onNormalTick(LocalPlayer* actor) override;
	PopCounter();
};