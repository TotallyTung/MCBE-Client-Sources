#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class RobotHolefiller : public IModule {
private:
	int wait = 5;
	int hold = 5;
	bool disableOnStop = true;
	#ifdef _DEBUG
	bool testOnSelf = false;
	#endif

	bool turnOnReady = false;
public:
	float entRange = 10.f;
	int wDel = 0;
	int hDel = 0;
	bool finW = false;

	std::vector<C_Entity*> targs;

	RobotHolefiller();
	~RobotHolefiller();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onWorldTick(C_GameMode* gm) override;
	virtual void onTick(C_GameMode* gm) override;
};