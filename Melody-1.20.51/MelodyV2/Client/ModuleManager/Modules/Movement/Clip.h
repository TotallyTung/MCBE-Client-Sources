// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class Clip : public Module {
private:
	float Clipvalue = 0.f;
public:

	Clip();
	virtual void onDisable() override;
	virtual void onNormalTick(Actor* actor) override;
};
