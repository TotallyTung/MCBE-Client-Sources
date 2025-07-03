// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class ChestStealler : public Module {
private:
	int Delay = 5;
public:
	ChestStealler();

	virtual void onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent& event);
};
