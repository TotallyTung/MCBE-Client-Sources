#pragma once
#include "Module.h"

class CustomFont : public IModule {
public:
	SettingEnum fontMode = SettingEnum(this);
	SettingEnum shadowMode = SettingEnum(this);
	CustomFont();
	virtual const char* getModuleName() override;
	virtual bool isEnabled() override;
	virtual bool isVisible() override;
};