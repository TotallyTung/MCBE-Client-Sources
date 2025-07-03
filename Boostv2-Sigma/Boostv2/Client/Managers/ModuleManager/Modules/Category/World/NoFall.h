#pragma once

#include "../../ModuleBase/Module.h"

class NoFall : public Module {
public:
	NoFall();
	std::string getModeText() override;
	~NoFall();

	// Inherited via IModule
	virtual void onSendPacket(Packet* packet) override;
};