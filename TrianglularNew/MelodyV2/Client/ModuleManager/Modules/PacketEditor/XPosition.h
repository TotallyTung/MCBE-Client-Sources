#pragma once
#include "../Module.h"

class XPosition : public Module {
private:
	float value = 0.f;
public:

	XPosition();
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
};
