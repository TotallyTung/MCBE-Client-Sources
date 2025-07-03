// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class Phase : public Module {
public:

	Phase();
	virtual void onDisable() override;
	virtual void onNormalTick(Actor* actor) override;
};
