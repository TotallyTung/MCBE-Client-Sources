#pragma once
#include "../../ModuleBase/Module.h"

class Criticals : public Module {
private:
	int mode = 0;
	float range = 7;
	float val = -0.015;
	int tick = 0;
public:
	Criticals();
	std::string getModeText() override;
	void onNormalTick(LocalPlayer* localPlayer) override;
	void onSendPacket(Packet* packet) override;
	void onEnable() override;
};