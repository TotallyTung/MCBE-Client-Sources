#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class PopCounter : public IModule {
public:
	bool sendchat = false;
	bool cys;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onJoiningServer();
	PopCounter();
};