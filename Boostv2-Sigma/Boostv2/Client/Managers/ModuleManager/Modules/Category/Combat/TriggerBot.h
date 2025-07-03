#pragma once
#include "../../ModuleBase/Module.h"

class TriggerBot : public Module {
private:

public:
	TriggerBot();

	virtual void onNormalTick(LocalPlayer* localPlayer) override;
};
