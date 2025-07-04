#pragma once
#include "../Module.h"

class ElytraFly : public Module {
public:
	float Speed = 4.f;
	ElytraFly();

	virtual void onNormalTick(Actor* actor) override;
};
