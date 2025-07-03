#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"

class BreakHighlight : public IModule {
public:
	const char* name = "BreakHighlight";
	vec3_ti renderPos = vec3_ti(0, 0, 0);
	bool renderProgress = false;
	float progress = 0.f;
	int face = 0;
	SettingEnum render = this;
	SettingEnum anim = this;
	float opacity = 0.2f;
	BreakHighlight();
	virtual const char* getRawModuleName() override;
	virtual const char* getModName();
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	virtual void onImGuiRender(ImDrawList* d);
};
