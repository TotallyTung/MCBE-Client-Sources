#pragma once
#include "Module.h"
class Chat : public IModule {
public:
	Chat();
	~Chat();
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet) override;
};