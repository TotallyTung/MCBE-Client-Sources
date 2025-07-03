#pragma once

#include "Module.h"

class CustomHotbar : public IModule {
public:
	SettingEnum hotbartype = this;
	SettingEnum smooth = this;
	float speed = 0.1f;
	CustomHotbar();
	~CustomHotbar();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
