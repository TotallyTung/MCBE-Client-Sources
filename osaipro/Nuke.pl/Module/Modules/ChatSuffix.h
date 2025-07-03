#pragma once
#include "../../Utils/Utils.h"
#include "../../Utils/FontChange.h"
#include "../ModuleManager.h"
#include "Module.h"

class ChatSuffix : public IModule {
public:

	ChatSuffix();
	~ChatSuffix();
	bool suffix = true;
	bool reset = false;
	int bypasslength = 4;
	SettingEnum suffixchanger = this;
	SettingEnum ColorChanger = this;
	SettingEnum bypass = this;
	SettingEnum fontchanger = this;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onSendPacket(Packet* packet) override;
};