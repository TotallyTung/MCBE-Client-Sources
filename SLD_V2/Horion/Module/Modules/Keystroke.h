#pragma once
#include "Module.h"

class Keystroke : public IModule {
public:
	Keystroke();
	~Keystroke();

	bool keystrokes = true;
	

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	static void drawKeystroke(char key, const vec2_t& pos);
	static void drawLeftMouseKeystroke(vec2_t pos);
	static void drawRightMouseKeystroke(vec2_t pos);
};