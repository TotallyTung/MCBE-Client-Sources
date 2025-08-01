#pragma once
#include "../../../../Utils/TargetUtil.h"
#include "../../../../SDK/CCamera.h"
#include "../../ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include "../Module.h"

class ESP : public IModule {
public:
	bool itemESP = false;
	bool tracers = false;
	bool mobs = false;
	bool is2D = false;

	SettingEnum mode = this;

	void onLevelRender() override;
	virtual const char* getRawModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	ESP();
};