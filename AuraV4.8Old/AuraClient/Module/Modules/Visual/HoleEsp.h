#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class HoleEsp : public IModule {
private:
	bool obsidian = true;
	bool bedrock = true;
	float Opacity = 1.f;
	float lineOpacity = 0.6f;
	bool outline = true;

public:
	int range = 5;
	float LineSize = 0.4f;

	HoleEsp();
	~HoleEsp();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	//virtual const char* getModName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	//virtual void onTick(C_GameMode* gm) override;
	//virtual const char* getRawModuleName() override;
};