#pragma once
#include "../../ModuleBase/Module.h"

class Fly : public Module {
private:
	bool smborder = false;
	bool bypass = false;
	bool hvhair = false;
	float hSpeed = 1.f;
	float stuck = 1.f;
	float currentSpeed = 1.f;
	int tickCounter = 1;
	float vSpeed = 0.5f;
	float Yset = 0.5f;
	float Glide = -0.02f;

public:
	Fly();

	void onSendPacket(Packet* packet, bool& shouldCancel);
	void onNormalTick(LocalPlayer* localPlayer) override;
	void onSendPacket(Packet* packet);
};