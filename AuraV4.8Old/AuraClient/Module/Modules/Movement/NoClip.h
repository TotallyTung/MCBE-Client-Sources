#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"

class NoClip : public IModule {
private:
	float speed = 0.325f;
	float upanddown = 0.6f;
	vec3_t oldPos;
	bool NoPacket = false;

public:
	NoClip();
	~NoClip();

	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm);
	virtual void onMove(C_MoveInputHandler* input);
	virtual void onEnable();
	virtual void onDisable();
};
