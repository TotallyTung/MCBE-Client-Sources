#pragma once
#include "../../ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include "../Module.h"

class Camera : public IModule {
public:
	bool fullbright = false;
	bool nohurtcam = false; 
	bool tint = false;
	bool synced = false;

	float* gammaPtr = nullptr;
	int intensity = 25;
	float red = 0.f;
	float green = 0.f;
	float blue = 0.f;
	float tintopacity = 100.f;

	//virtual void onPlayerTick(C_Player* plr);
	virtual void onWorldTick(C_GameMode* gm);
	virtual const char* getRawModuleName();
	virtual void onTick(C_GameMode* gm);
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	virtual void onDisable();
	virtual void onEnable();
	Camera();
};