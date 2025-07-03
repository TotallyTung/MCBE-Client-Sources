#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"

class Jetpack : public Module {
private:
	int delay = 0;
	float speedMod = 1;
	//bool hold = true;

public:
	bool isBypass = false;
	MovePlayerPacket* jetpack = nullptr;
	Jetpack();
	~Jetpack();

	virtual void onEnable();
	virtual void onNormalTick(Actor* actor);
};
