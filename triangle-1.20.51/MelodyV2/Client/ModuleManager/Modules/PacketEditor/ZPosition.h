#pragma once
#include "../Module.h"

class ZPosition : public Module {
private:
	float value = 0.f;
public:

	ZPosition();
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
};
