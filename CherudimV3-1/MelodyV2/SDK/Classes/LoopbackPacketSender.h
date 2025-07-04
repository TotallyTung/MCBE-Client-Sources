#pragma once
#include "Packet.h"

class LoopbackPacketSender {
public:
	virtual ~LoopbackPacketSender();
	virtual __int64 send(Packet* packet);
	virtual __int64 sendToServer(Packet* packet);
};