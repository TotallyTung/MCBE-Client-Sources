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
	bool HideMinecraftNametags = true;
	int rf = 0;
	int gf = 255;
	int bf = 255;

	int re = 160;
	int ge = 255;
	int be = 255;

	MC_Color friendscolor = MC_Color(rf, gf, bf);
	MC_Color Enemycolor = MC_Color(re, ge, be);

	bool underline = true;
	bool inview = true;
	float opacity = 1.f;
	bool ingameNametagSetting = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
};
