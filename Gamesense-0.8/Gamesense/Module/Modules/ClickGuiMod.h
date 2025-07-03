#pragma once
#include "../../Menu/ClickGui.h"
#include "../../Config/ConfigManager.h"
#include "../../Config/SettingManager.h"
#include "Module.h"

class ClickGUIMod : public IModule {
private:
public:
	int openTimeOffset = 0;
	bool showHudEditor = false;
	bool hasOpenedGUI = false; // start with hudeditor
	std::vector<std::filesystem::directory_entry> configs;
	bool settingOpened = false;
	float red = 0.f;
	float green = 0.f;
	float blue = 0.f;
	float tintopacity = 100.f;
	bool headerAnimation = false;
	bool openAnimation = false;
	bool isSettingOpened = false;
	bool animations = true;
	bool enabled = true;
	bool sounds = true;
	bool cFont = true;
	float animation = 1.f;
	float openAnim = 1.f;
	int openTime = 0;
	int opacity = 175;
	int t = 0;
	int opacityText = 0;
	int opacityReal = 0;
	int scrollSpeed = 1;
	float opacityBackGround;
	bool animation2 = true;
	bool animation1 = true;
	SettingEnum scrolling = this;
	SettingEnum desc;
	SettingEnum tint = this;

	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx);
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	virtual void onPlayerTick(C_Player* plr);
	virtual void onLoadConfig(void* conf);
	virtual void onSaveConfig(void* conf);
	virtual const char* getRawModuleName();
	virtual void onTick(C_GameMode* gm);
	virtual bool allowAutoStart();
	virtual void onDisable();
	virtual void onEnable();
	ClickGUIMod();
};
