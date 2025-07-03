#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Blink : public IModule
{
private:
	std::vector<vec3_t> blockBelow;
	std::vector<PlayerAuthInputPacket *> PlayerAuthInputPacketHolder;
	std::vector<C_MovePlayerPacket *> MovePlayerPacketHolder;

public:
	bool hold = false;
	bool trail = true;
	std::string name = "Blink";
	inline std::vector<PlayerAuthInputPacket *> *getPlayerAuthInputPacketHolder() { return &PlayerAuthInputPacketHolder; };
	inline std::vector<C_MovePlayerPacket *> *getMovePlayerPacketHolder() { return &MovePlayerPacketHolder; };
	virtual void onTick(C_GameMode *gm);
	virtual void onLevelRender();
	virtual void onDisable();
	virtual bool isHoldMode();
	virtual const char *getRawModuleName();
	virtual const char *getModuleName();
	Blink();
	~Blink();
};