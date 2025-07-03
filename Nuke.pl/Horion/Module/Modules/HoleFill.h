#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class HoleFill : public IModule {
private:
	bool obsidian = true;
	bool bedrock = true;
	bool onclick = false;
	SettingEnum mode = this;
public:
	int range = 5;

	bool tryHoleFill(Vec3i blkPlacement);
	bool canpla(Vec3i blockPos);

	HoleFill();
	~HoleFill();
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};