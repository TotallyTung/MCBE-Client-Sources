#pragma once
#include "../../ModuleBase/Module.h"

class AutoTool : public Module {
private:
	int getBestDestoryItemSlot(Block* block);
public:
	AutoTool();
	void onNormalTick(LocalPlayer* localPlayer) override;
};