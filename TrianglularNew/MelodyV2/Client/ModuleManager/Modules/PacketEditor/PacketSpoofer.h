#pragma once
#include "../Module.h"

class PacketSpoofer : public Module {
private:
	float value = 0.f;
public:

	PacketSpoofer();
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
};
