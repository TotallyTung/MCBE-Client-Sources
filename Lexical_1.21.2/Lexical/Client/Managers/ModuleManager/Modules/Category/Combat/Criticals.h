#pragma once
#include "../../ModuleBase/Module.h"

class Criticals : public Module {
private:
	int mode = 0;
public:
	Criticals();
	std::string getModeText() override;
	void onSendPacket(Packet* packet) override;
};