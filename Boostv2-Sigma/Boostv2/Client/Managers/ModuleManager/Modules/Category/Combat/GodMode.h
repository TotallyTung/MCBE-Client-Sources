#pragma once
#include "../../ModuleBase/Module.h"

class GodMode : public Module {
public:
	GodMode();
	void onNormalTick(LocalPlayer* localPlayer) override;
};