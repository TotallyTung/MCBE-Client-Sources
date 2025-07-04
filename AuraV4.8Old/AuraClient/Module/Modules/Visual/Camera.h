#pragma once
#include "../../ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include "../Module.h"

class Camera : public IModule {
public:
	bool fullbright = false;
	bool nohurtcam = false; 

	float* gammaPtr = nullptr;
	int intensity = 25;

	//virtual void onPlayerTick(C_Player* plr);
	virtual void onWorldTick(C_GameMode* gm);
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm);
	virtual void onDisable();
	virtual void onEnable();
	Camera();
};