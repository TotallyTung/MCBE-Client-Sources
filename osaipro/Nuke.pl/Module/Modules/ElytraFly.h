#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class ElytraFly : public IModule {
public:
	float upanddown = 0.6f;
	float glideMod = -0.00f;
	float glideModEffective = 0;
	bool flying = false;
	int upkey = 38;
	int downkey = 40;

	ElytraFly();
	~ElytraFly();
	virtual void onKey(int key, bool isDown, bool& shouldCancel) override;
	virtual void onTick(GameMode* gm) override;
	virtual void onMove(MoveInputHandler* input) override;
	virtual const char* getModuleName();
};