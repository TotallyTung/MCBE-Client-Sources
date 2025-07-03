#pragma once

#include "Module.h"

class ConsoleMod : public IModule {
public:
	ConsoleMod();
	~ConsoleMod();
	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	virtual void onImGuiRender(ImDrawList* d) override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
};
