#pragma once

#include "Module.h"

class DragCoordinates : public IModule {
public:
	DragCoordinates();
	~DragCoordinates();
	virtual const char* getModuleName() override;
	virtual void onDisable() override;
	virtual void onEnable() override;
	virtual void onImGuiRender(ImDrawList* d) override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
};
