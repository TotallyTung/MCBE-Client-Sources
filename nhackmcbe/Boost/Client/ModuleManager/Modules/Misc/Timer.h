#pragma once
#include "../Module.h"

class Timer : public Module {
private:
	int tps = 20;
public:

	Timer();
	virtual void onNormalTick(Actor* actor) override;
	virtual void onDisable() override;
};
