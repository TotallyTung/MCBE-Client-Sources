#pragma once
#include "../../ModuleBase/Module.h"

class Switcher : public Module {
private:

public:
	Switcher();

	bool SpoofSwitch(int slot);

	virtual void onNormalTick(LocalPlayer* localPlayer) override;
};
