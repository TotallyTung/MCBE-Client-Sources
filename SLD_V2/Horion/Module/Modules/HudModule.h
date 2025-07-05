#pragma once
#include "Module.h"

class HudModule : public IModule {
public:
	HudModule();
	~HudModule();

	bool tabgui = true;
	bool arraylist = true;
	bool arraylistbottom = true;
	bool arraylisttop = true;
	bool clickToggle = true;
	bool watermark = false;
	bool coordinates = false;
	bool keybinds = true;
	bool displayArmor = true;
	bool keystrokes = true;
	bool fps = true;
	bool cps = true;
	bool welcomer = true;
	bool alwaysShow = false;

	float welcomerX = 300.0f;
	float welcomerY = 50.0f;
	float scale = 1.f;

	int customr = 255;
	int customg = 0;
	int customb = 0;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	static void drawKeystroke(char key, const vec2_t& pos);
	static void drawLeftMouseKeystroke(vec2_t pos);
	static void drawRightMouseKeystroke(vec2_t pos);
};
