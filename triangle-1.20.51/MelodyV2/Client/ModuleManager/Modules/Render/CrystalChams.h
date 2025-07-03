#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class CrystalChams : public Module {
public:
	bool itemESP = false;
	bool tracers = false;
	bool mobs = false;
	bool is2D = false;
	bool isEndcr;

	int mode;

	virtual void onRender(MinecraftUIRenderContext* renderCtx) override;
	CrystalChams();
};