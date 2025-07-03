#pragma once
#include "../../Utils/DrawUtils.h"
#include "../../Utils/ColorUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class ArrayList : public IModule {
private:
	SettingEnum mode = this;

	float spacing = 0.f;
	float scale = 1.f;
	float opacity = 0.2f;
	int size = 35.f;
public:
	bool bottom = false; // puts da arraylisrs in da butom

	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx);
	virtual void onImGuiRender(ImDrawList* d);
	virtual const char* getRawModuleName();
	ArrayList();
};
