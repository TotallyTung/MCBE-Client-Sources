#pragma once
#include "../../ModuleBase/Module.h"

class CameraTweaks : public Module {
public:
	bool clip = false;
	float distance = 4.f;
private:
	void* targetAddress = nullptr;
	char ogBytes[5] = {};
	bool nopedBytes = false;
public:
	CameraTweaks();
	void onDisable() override;
	void onClientTick() override;
};