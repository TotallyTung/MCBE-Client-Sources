#pragma once
#include "../Module.h"

class VerticalPosition : public Module {
private:
	float value = 0.f;
public:

	VerticalPosition();
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
};
