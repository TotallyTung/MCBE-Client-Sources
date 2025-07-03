#pragma once
#include "../../ModuleBase/Module.h"

class Timer : public Module {
private:
	int timerValue = 40;
public:
	Timer();

	void onDisable() override;
	void onClientTick() override;
};