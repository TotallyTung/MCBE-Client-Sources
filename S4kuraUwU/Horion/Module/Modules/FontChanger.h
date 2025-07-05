#pragma once
#include "Module.h"
class FontChanger : public IModule {
public:
	SettingEnum Fonts;
	FontChanger();
	~FontChanger();
	int font = 0;
	// Inherited via IModule
	virtual const char* getModuleName() override;
};