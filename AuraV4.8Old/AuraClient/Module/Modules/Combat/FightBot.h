#pragma once
#pragma once
#pragma once
#include "../../Utils/TargetUtil.h"
#include "../Module.h"
#include "../../ModuleManager.h"

class FightBot : public IModule {
private:
	bool silent = true;

public:
	int targ = 100;
	bool isMulti = true;
	bool isMobAura = false;
	bool targetListA = false;
	bool sexy = true;
	vec2_t joe;

	FightBot();
	~FightBot();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onLevelRender() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onSendPacket(C_Packet* packet, bool& cancelSend);
};
