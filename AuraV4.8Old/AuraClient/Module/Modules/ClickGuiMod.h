#pragma once
#include "../../Menu/HudEditor.h"
#include "../../Menu/ClickGui.h"
#include "Module.h"

class ClickGUIMod : public IModule {
public:
	//bool showHudEditor = false;
	bool hasOpenedGUI = false; // start with hudeditor

	bool headerAnimation = false;
	bool openAnimation = false;
	bool showTooltips = true;
	bool rainbowColor = false;
	bool animations = true;
	bool enabled = true;
	bool sounds = false;
	bool cFont = false;
	bool outline = true;
	bool line = false;

	bool ModuleOutline = false;
	bool Crossline = false;

	float animation = 1.f;
	float openAnim = 1.f;
	int t = 0;

	//SettingEnum theme = this;
	//SettingEnum color = this;

	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx);
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	virtual void onPlayerTick(C_Player* plr);
	virtual void onLoadConfig(void* conf);
	virtual void onSaveConfig(void* conf);
	virtual const char* getModuleName();
	//virtual void onTick(C_GameMode* gm);
	virtual bool allowAutoStart();
	virtual void onDisable();
	virtual void onEnable();
	ClickGUIMod();
};