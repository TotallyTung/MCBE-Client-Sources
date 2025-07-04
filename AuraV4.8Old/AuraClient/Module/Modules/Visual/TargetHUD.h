#pragma once
//#include "../../../../Utils/TargetUtil.h"
//#include "../../../../SDK/CAttribute.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class TargetHUD : public IModule {
private:
	int opacity = 100;
	bool outlinetgh = true;
public:
	float rangeTGH = 8.f;
	bool targetListEmpty = true;
	float targetLen = 0.f;
	float positionX = 10;
	float positionY = 40;

	SettingEnum mode = this;

	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx);
	virtual const char* getModuleName();
	virtual void onTick(C_GameMode* gm);
	TargetHUD();
};