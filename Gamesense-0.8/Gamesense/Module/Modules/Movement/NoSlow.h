#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class NoSlow : public IModule {
private:
	uint8_t* opcode1 = 0;
	uint8_t* opcode = 0;
	bool web = true;
	bool spoof = false;
	bool antii = false;
public:
	std::string name = "NoSlow";
	SettingEnum mode = this;

	virtual const char* getRawModuleName();
	virtual void onTick(C_GameMode* gm);
	virtual const char* getModuleName();
	virtual void onDisable();
	virtual void onEnable();
	virtual void onMove(C_MoveInputHandler* input) override;
	NoSlow();
};