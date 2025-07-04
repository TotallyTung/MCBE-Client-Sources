#pragma once
#include "../Module.h"

class AutoTools : public Module {
public:
	AutoTools();

	virtual void onNormalTick(Actor* actor) override;
};
