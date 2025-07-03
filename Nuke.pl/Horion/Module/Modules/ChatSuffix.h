#pragma once
#include "../../Utils/Utils.h"
#include "../ModuleManager.h"
#include "Module.h"
class ChatSuffix : public IModule {
public:
	ChatSuffix();
	~ChatSuffix();
	bool green = true;
	bool bypass1 = true;
	bool bypass2 = true;
	bool suffix = true;
	bool reset = false;
	int bypass1length = 4;
	int bypass2length = 9;
	SettingEnum suffixenum = this;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onSendPacket(Packet* packet) override;
};