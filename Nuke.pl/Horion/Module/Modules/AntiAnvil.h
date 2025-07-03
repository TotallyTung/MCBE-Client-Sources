#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class AntiAnvil : public IModule {
private:
	int slottd2 = 0;
	float height = 1.0f;  // Added height variable

public:
	bool spam = false;
	bool spam2 = false;
	AntiAnvil();

	virtual void onTick(GameMode* gm) override;
	virtual const char* getModuleName() override;
	virtual const char* getModName() override;
	bool isFallingAnvilAbovePlayer(Player* plr) const;
	virtual void onEnable() override;
	virtual void onLevelRender() override;
};
