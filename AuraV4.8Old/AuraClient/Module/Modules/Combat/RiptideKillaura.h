#pragma once
#include "../Module.h"

class RiptideKillaura : public IModule {
public:
	RiptideKillaura();
	~RiptideKillaura();

	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend);
};
#pragma once
