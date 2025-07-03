#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Watermark : public IModule
{
private:
	// float scale = 1.f;
	// int opacity = 150;
public:
	// bool outlinec = true;
	bool showVersion = true;
	int spacing = 70;
	bool gradient = true;
	float alpha = 255.f;
	int textSize = 40;
	bool isFirstTime = true;
	int magicOpacity = 255.f;
	float magicOpacity2 = 1.f;
	int blackDelay = 80;
	int fTDel = 0;
	SettingEnum mode;

	virtual void onPostRender(C_MinecraftUIRenderContext *renderCtx);
	virtual const char *getRawModuleName();
	virtual void onEnable() override;
	virtual void onImGuiRender(ImDrawList* d);
	Watermark();
};