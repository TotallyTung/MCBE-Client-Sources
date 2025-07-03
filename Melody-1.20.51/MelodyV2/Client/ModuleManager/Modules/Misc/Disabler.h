// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class Disabler : public Module {
private:
	int Mode = 0;
public:
	Disabler();
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
};