#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class AutoSneak : public IModule {
public:
	bool silentlol = true;
	std::string name = "AutoSneak";
	virtual void onSendPacket(Packet* packet);
	virtual void onPlayerTick(Player* plr);
	virtual const char* getModuleName();
	virtual const char* getModName();
	virtual void onTick(GameMode* gm);
	virtual void onEnable();
	virtual void onDisable();
	AutoSneak();
};