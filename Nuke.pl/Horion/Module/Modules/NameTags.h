#pragma once

#include <set>

#include "../../../SDK/Camera.h"
#include "../../DrawUtils.h"
#include "../../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class NameTags : public IModule {
public:
	std::set<std::string> nameTags;
	bool displayArmor = true;
	NameTags();
	~NameTags();
	bool underline = true;
	bool inview = true;
	float opacity = 1.f;
	bool ingameNametagSetting = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
};
