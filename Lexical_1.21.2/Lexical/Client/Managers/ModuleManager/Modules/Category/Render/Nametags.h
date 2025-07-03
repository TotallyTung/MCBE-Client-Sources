#pragma once
#include "../../ModuleBase/Module.h"

class NameTags : public Module {
private:
	float opacity = 0.4f;
	bool showPops = false;
	bool showItems = true;
	bool showDura = true;
	bool showSelf = true;
	bool underline = true;
	bool dynamicSize = false;
public:
	NameTags();
	virtual void onD2DRender() override;
	virtual void onMCRender(MinecraftUIRenderContext* renderCtx) override;
};