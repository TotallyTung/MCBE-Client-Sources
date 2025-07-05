#pragma once
#include "../ModuleManager.h"
#include "Module.h"

class PlayerList : public IModule {
private:
	float range = 255;

	
public:
	float positionX, positionY = 0;
	vec4_t rectPos2;

	SettingEnum theme = this;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx);
	virtual void onTick(C_GameMode* gm);
	virtual const char* getModuleName();
	virtual void onDisable();
	virtual void onEnable();
	PlayerList();
};