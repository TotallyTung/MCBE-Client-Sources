#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class PlayerListx : public IModule {
public:
	PlayerListx();
	~PlayerListx();

	float scale = 0.9f;
	bool armour = true;
	bool pos = false;
	bool dist = true;
	float posX = 262.5f;
	float posY = 8.5f + (10.f * 0);
	int rf = 0;
	int gf = 255;
	int bf = 0;

	int re = 255;
	int ge = 255;
	int be = 255;

	int rpd = 100;
	int gpd = 0;
	int bpd = 255;
	// Inherited via IModule
	virtual void onDisable();
	virtual void onJoiningServer();
	virtual void onEnable();
	virtual const char* getModuleName();
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx);
	//virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};
