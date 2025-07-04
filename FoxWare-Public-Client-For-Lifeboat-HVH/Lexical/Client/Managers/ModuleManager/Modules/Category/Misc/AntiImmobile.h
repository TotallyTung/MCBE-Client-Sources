#pragma once
#include "../../ModuleBase/Module.h"

class AntiImmobile : public Module {
public:
	AntiImmobile();
	void onSendPacket(Packet* packet) override;
};