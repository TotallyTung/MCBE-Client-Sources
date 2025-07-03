// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class ElytraFly : public Module {
public:
	float Speed = 4.f;
	ElytraFly();

	virtual void onNormalTick(Actor* actor) override;
};
