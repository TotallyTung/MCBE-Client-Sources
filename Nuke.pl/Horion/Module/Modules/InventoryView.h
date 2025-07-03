#pragma once
#include "Module.h"

class InventoryView : public IModule {
private:
	float posX = 100.f;
	float posY = 100.f;
	int alpha = 135;
	bool outline = true;

public:
	InventoryView();

	virtual const char* getModuleName() override;
	virtual bool isVisible() override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
};