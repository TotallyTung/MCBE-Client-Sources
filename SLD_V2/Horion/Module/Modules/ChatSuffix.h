#pragma once
#include "Module.h"
class ChatSuffix : public IModule {
private:
	SettingEnum mode;
	bool suffix = true;
	bool bypass = false;
	bool greenText = false;
	bool autoez = false;
	std::string custom = "Soldiers";
	// std::string end2;

public:
	int kc = 0;
	int dc = 0;
	bool foundPlayer = false;
	bool isDead = false;
	bool antiSpamCheck = false;
	inline std::string &getSuffix() { return custom; };
	ChatSuffix();

	// Inherited via IModule
	virtual const char *getModuleName() override;
	virtual void onTick(C_GameMode *gm) override;
	virtual void onSendPacket(C_Packet *packet) override;
};