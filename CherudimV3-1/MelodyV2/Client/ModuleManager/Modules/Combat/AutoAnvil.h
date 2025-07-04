#pragma once
#include "../Module.h"

class AutoAnvil : public Module {
private:
	bool attackMob = false;
public:
	std::vector<Actor*> targetList;
	AutoAnvil();

	virtual void onNormalTick(Actor* actor) override;
};