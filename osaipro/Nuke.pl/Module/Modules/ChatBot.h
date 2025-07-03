#pragma once
#include "../../Utils/Utils.h"
#include "../ModuleManager.h"
#include "Module.h"

class ChatBot : public IModule {
public:

	ChatBot();
	~ChatBot();
	bool cmd1 = false;
	bool cmd2 = false;
	bool cmd3 = false;
	bool cmd4 = false;
	bool cmd5 = false;
	bool cmd6 = false;
	bool cmd7 = false;
	bool cmd8 = false;
	bool filter = false;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onEnable();
	virtual void onSendPacket(Packet* packet) override;
};