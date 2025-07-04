#pragma once
#include "../Module.h"

class AutoEat : public Module {
public:
	AutoEat();

	virtual void onNormalTick(Actor* actor) override;
};
