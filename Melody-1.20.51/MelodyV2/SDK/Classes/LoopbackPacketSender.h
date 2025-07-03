// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "Packet.h"

class LoopbackPacketSender {
public:
	virtual ~LoopbackPacketSender();
	virtual __int64 send(Packet* packet);
	virtual __int64 sendToServer(Packet* packet);
};