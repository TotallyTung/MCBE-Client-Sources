#pragma once
#include "Module.h"

class Watermark2 : public IModule {
public:
	Watermark2();
	~Watermark2();

	bool outline = true;
	bool background = true;
	bool top = true;
	bool bottom = false;
	bool rgba = true;


	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};