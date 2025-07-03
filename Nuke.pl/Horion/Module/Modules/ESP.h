#pragma once
#include "../../../SDK/Camera.h"
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "../../DrawUtils.h"
#include "Module.h"

class ESP : public IModule {
public:
	bool itemESP = false;
	bool tracers = false;
	bool mobs = false;
	bool is2D = false;

	SettingEnum mode = this;

	void onLevelRender() override;
	virtual const char* getModuleName() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	ESP();
};