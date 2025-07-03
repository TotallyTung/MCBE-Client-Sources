#pragma once
#include "../../../Utils/Json.hpp"
#include "../ModuleManager.h"
#include "Module.h"
class CombatInfo : public IModule {
public:
	CombatInfo();
	~CombatInfo();
	bool showinclickgui = true;
	float posx = 0.f;
	float posy = 100.f;
	float scale = 1.f;
	bool showkb__ = true;
	std::vector<std::pair<std::string, std::string>> ModName_NameAlias;
	virtual const char* getModuleName() override;
	virtual void onImGuiRender(ImDrawList* d) override;
	virtual void onLoadConfig(void* confVoid) override;
	virtual void onSaveConfig(void* confVoid) override;
};