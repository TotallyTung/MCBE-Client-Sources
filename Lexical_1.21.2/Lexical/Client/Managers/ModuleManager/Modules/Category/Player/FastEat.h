#pragma once
#include "../../ModuleBase/Module.h"

class FastEat : public Module {
private:
	int duration = 9;
public:
	FastEat();

	virtual void onDisable() override;
	virtual void onNormalTick(LocalPlayer* actor) override;
};
