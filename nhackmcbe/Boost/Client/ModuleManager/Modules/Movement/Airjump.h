#pragma once
#include "../Module.h"

class Airjump : public Module {
private:
public:

	Airjump();
	virtual void onNormalTick(Actor* actor) override;
};
