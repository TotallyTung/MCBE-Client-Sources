// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class AntiCrystal : public Module {
private:
	float value = 0.f;
public:

	AntiCrystal();
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
};
