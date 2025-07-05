#pragma once
#include "Module.h"
#include "../ModuleManager.h"
class Criticals : public IModule {
public:
	Criticals();
	~Criticals();
	bool hurttime = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet) override;
};
