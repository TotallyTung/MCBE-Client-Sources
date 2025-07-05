#pragma once
#include "Module.h"

class Coordinates : public IModule {
public:
	Coordinates();
	~Coordinates(); 

	float scale = 1.f;
	float coordsX = 0.00f;
	float coordsY = 340.84f;
	bool rgba = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};