#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class HoleTP : public IModule {
private:
	bool obsidian = true;
	bool bedrock = true;
	bool onclick = false;
public:
	int range = 5;

	bool tryHoleTP(Vec3i blkPlacement);
	bool canpla(Vec3i blockPos);

	HoleTP();
	~HoleTP();
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};