#pragma once
#include "../Module.h"

class AutoOffhand : public Module {
private:
	int itemMode = 0;
public:
	AutoOffhand();

	virtual void onNormalTick(Actor* actor) override;
};
