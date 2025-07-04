#pragma once
#include "../Module.h"

class ChatSuffix : public Module {
public:
	ChatSuffix();

	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
};