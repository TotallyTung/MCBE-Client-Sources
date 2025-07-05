#pragma once
#include "Module.h"
#include "../ModuleManager.h"
class InventoryView : public IModule {
public:
	InventoryView();
	~InventoryView();
	SettingEnum Color;
	bool rainbow = false;
	bool pastel = false;
	float xVal = 28.f;
	float invX = 100.f;
	float invY = 100.f;
	bool outline = false;
	static float rFloat;
	static float bFloat;
	static float gFloat;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};