#pragma once
#include "Module.h"

class HudModule : public IModule {
public:
	HudModule();
	~HudModule();

	float scale2 = 0.45f;
	bool rainbow = false;
	bool pastel = false;

	std::string name = "";
	std::string name2 = "";
	bool Hello = false;
	bool brooda = false;
	bool badman = true;

	bool Rainbow = false;

	float r1 = 255.f;
	float g1 = 55.f;
	float b1 = 255.f;

	float r2 = 255.f;
	float g2 = 255.f;
	float b2 = 255.f;
	float sped = 50.f;

	float speed = 0.f;

	SettingEnum TextMode;
	SettingEnum rainBow;

	static int rFloat;
	static int bFloat;
	static int gFloat;

	bool tabgui = false;
	//bool arraylist = false;
	//bool arraylistbottom = true;
	//bool arraylisttop = false;
	bool clickToggle = true;
	bool watermark2 = true;
	bool coordinates = true;
	bool keybinds = true;
	bool displayArmor = true;
	bool keystrokes = false;
	bool fps = true;
	bool cps = true;
	bool bps = true;
	bool alwaysShow = false;


	float scale = .75f;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
	static void drawKeystroke(char key, const vec2_t& pos);
	static void drawLeftMouseKeystroke(vec2_t pos);
	static void drawRightMouseKeystroke(vec2_t pos);
};
