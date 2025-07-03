#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class AnchorAura : public IModule {
public:
	AnchorAura();
	~AnchorAura();
	int range = 5;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	static void onJoiningServer();
	static void onLevelRender();
};