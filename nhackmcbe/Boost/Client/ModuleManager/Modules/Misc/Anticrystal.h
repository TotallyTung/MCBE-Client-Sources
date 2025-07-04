#pragma once
#include "../Module.h"

class AntiCrystal : public Module {
private:
	float value = 0.f;
	int Mode;
public:

	AntiCrystal();
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
};
