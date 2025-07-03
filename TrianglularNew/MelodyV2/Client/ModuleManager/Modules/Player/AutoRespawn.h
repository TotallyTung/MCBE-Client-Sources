#pragma once
#include "../Module.h"

class AutoRespawn : public Module {
public:

	AutoRespawn();
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
};