#pragma once
#include "../Module.h"

class AutoSprint : public Module {
private:
	int Mode = 0;
public:

	AutoSprint();
	virtual void onNormalTick(Actor* actor) override;
};
