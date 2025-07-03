#pragma once
#include "../../ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include "../Module.h"

class BlockOutline : public IModule {
private:
	float red, green, blue = 1.f;
	float Opacity = 1.f;
	float OutOpacity = 1.f;
	bool sync = true;
	bool fill = true;
	float width = 0.5f;
public:
	SettingEnum mode = this;
	SettingEnum boxtype = this;

	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	virtual const char* getRawModuleName();
	BlockOutline();
};