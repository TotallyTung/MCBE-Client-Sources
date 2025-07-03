// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class AutoSprint : public Module {
private:
	int Mode = 0;
public:

	AutoSprint();
	virtual void onNormalTick(Actor* actor) override;
};
