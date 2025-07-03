#pragma once
#include "../../ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include "../Module.h"

class Interface : public IModule
{
public:
	std::string versionStr = "0.9.1";

public:
	// Settings
	bool keystrokes = false;
	float saturation = 1.f;
	float brightness = 1.f;
	bool tabGUI = false;
	int opacity = 150;
	float coloropacity = 255.f;
	int glowlayers = 12;
	float glowopacity = 0.75;
	int spacing = 70;

	float scale = 1.f;

	// Colors
	SettingEnum color = this;
	SettingEnum Fonts = this;
	float r = 255.f, g = 0.f, b = 0.f;
	float r2 = 150.f, g2 = 0.f, b2 = 0.f;

	virtual const char *getRawModuleName();
	Interface();
};
