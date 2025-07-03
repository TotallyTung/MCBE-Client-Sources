#pragma once
#include "../ModuleManager.h"
#include "../../DrawUtils.h"
#include "Module.h"

class BreadCrumbs : public IModule {
private:
	std::vector<Vec3> blockBelow;
	bool clear = false;
public:
	SettingEnum mode = this;

	virtual const char* getModuleName();
	virtual void onTick(GameMode* gm);
	virtual void onLevelRender();
	virtual void onDisable();
	BreadCrumbs();
};