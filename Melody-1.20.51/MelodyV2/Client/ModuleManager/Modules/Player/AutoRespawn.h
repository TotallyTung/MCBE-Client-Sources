// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class AutoRespawn : public Module {
public:

	AutoRespawn();
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
};