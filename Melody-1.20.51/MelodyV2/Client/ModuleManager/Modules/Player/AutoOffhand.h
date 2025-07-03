// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class AutoOffhand : public Module {
private:
	int itemMode = 0;
public:
	AutoOffhand();

	virtual void onNormalTick(Actor* actor) override;
};
