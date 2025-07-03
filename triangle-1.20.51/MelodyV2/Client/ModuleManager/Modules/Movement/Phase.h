#pragma once
#include "../Module.h"

class Phase : public Module {
public:

	Phase();
	virtual void onDisable() override;
	virtual void onNormalTick(Actor* actor) override;
};
