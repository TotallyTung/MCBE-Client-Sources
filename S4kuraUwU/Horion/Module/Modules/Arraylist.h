#pragma once
#include "Module.h"

class ArrayList : public IModule {
public:
	SettingEnum Modes;
	ArrayList();
	~ArrayList();

	bool shadow = false;
	bool bottom = true;
	float opacity = 0.2f;
	bool bugs = false;
	bool rgba = false;
	bool outline = false;
	bool underline = false;
	float scale = 1.0f;
	bool pink = false;
	bool ice = false;
	virtual const char* getModuleName() override;
};