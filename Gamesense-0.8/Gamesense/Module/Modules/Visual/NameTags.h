#pragma once
#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../../Utils/DrawUtils.h"
#include "../Module.h"

class NameTags : public IModule {
public:
	std::set<std::string> nameTags;
	bool displayArmor = true;
	bool displayDurability = true;
	bool drawhealth = true;
	bool showItems = true;
	bool itemTags = true;
	//bool underline = true;
	bool showEnchantment = false;
	bool hideTags = false;
	bool showBar = true;
	bool showPercentage = true;
	bool fsync = false;
	float scalen = 0.6f;
	float fred = 0.f;
	float fgreen = 255.f;
	float fblue = 255.f;
	int opacity = 150;

	bool* ingameNametagSetting = nullptr;
	bool gotPrevSetting = false;
	bool lastSetting = true;

	SettingEnum renderMode = this;

	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	virtual const char* getRawModuleName();
	virtual void onDisable();
	NameTags();
};