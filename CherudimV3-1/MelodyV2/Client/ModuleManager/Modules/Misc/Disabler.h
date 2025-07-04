#pragma once
#include "../Module.h"

class Disabler : public Module {
private:
	int Mode = 0;
public:
	Disabler();
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
};