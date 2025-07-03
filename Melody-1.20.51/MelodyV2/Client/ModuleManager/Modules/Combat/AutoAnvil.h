// Created by Tony on 2024-10-10 10:01:54

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