#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class PlayerListx : public IModule {
public:
	PlayerListx();
	~PlayerListx();

	float scale = 0.9f;
	bool armour = true;

	// Inherited via IModule
	virtual void onDisable();
	virtual void onEnable();
	virtual const char* getModuleName();
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx);
	//virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};
