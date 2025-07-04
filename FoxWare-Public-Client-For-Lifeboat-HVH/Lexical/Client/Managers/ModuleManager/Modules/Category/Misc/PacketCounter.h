#pragma once
#include "../../ModuleBase/Module.h"

class PacketCounter : public Module {
public:
	int pargets1 = 0;
	PacketCounter();
	void onSendPacket(Packet* packet) override;
	void onD2DRender() override;
};