#pragma once
#include "../Module.h"

class ChestStealler : public Module {
private:
	int Delay = 5;
public:
	ChestStealler();

	virtual void onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent& event);
};
