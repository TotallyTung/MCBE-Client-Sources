#pragma once

#include "Module.h"

class HudEditor : public IModule {
public:
	HudEditor();
	~HudEditor();
	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	virtual void onImGuiRender(ImDrawList* d) override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
};
