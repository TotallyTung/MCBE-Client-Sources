#pragma once
#include "../../ModuleBase/Module.h"

class ElytraFly : public Module {
public:
	float Speed = 4.f;
	int upKey = VK_TAB;
	int downKey = VK_SHIFT;
	ElytraFly();

	void onDisable() override;
	void onSendPacket(Packet* packet) override;
	virtual void onNormalTick(LocalPlayer* actor) override;
};
