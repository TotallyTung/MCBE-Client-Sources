#pragma once
#include "Module.h"

class InstaBreak : public IModule {
public:
	int delay = 5;

	SettingEnum Modes;
	InstaBreak();
	~InstaBreak();
	// Inherited via IModule
	virtual const char* getModuleName() override;
};
