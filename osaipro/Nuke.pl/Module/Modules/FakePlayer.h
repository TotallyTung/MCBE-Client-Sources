#pragma once
#include "../../DrawUtils.h"
#include "../ModuleManager.h"
#include "Module.h"

class FakePlayer : public IModule {
public:
	Vec3 FakePlayerPos = Vec3(0,0,0);
	std::string FakePlayerName = "FakePlayer";
	FakePlayer();
	~FakePlayer();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};