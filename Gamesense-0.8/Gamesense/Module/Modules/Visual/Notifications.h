#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Notifications : public IModule
{
public:
	int notifSize = 35;
	SettingEnum mode = this;

	virtual const char *getRawModuleName() override;
	Notifications();
};
