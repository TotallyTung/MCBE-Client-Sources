#pragma once
#include "../../ModuleBase/Module.h"

class AutoSprint : public Module {
private:
	int mode = 0;
	EnumSetting* modeEnum = nullptr;
public:
	AutoSprint();

	std::string getModeText() override;
	void onNormalTick(LocalPlayer* localPlayer) override;
};
