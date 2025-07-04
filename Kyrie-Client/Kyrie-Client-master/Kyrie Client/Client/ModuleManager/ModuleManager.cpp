#include "ModuleManager.h"
#include <algorithm>

ModuleManager::ModuleManager(Client* c) {
	this->client = c;

	{
		//Combat
		//Misc
		//Render
		this->moduleList.emplace_back(new FullBright());
		this->moduleList.emplace_back(new Swing());
		this->moduleList.emplace_back(new CustomFov());
		//Movement
		this->moduleList.emplace_back(new Sprint());
		//Player
		this->moduleList.emplace_back(new BlockReach());
		//Client
		this->moduleList.emplace_back(new Arraylist());
		this->moduleList.emplace_back(new ClickGui());
		this->moduleList.emplace_back(new Colors());
		this->moduleList.emplace_back(new CustomFont());
		this->moduleList.emplace_back(new HUD());
		this->moduleList.emplace_back(new Notifications());
		this->moduleList.emplace_back(new Unject());
		//Unknow

	}

	std::sort(moduleList.begin(), moduleList.end(), [](const std::shared_ptr<Module>& lhs, const std::shared_ptr<Module>& rhs) {
		return std::string{ *lhs->getModuleName() } < std::string{ *rhs->getModuleName() };
	});

	this->initialized = true;
	this->getModule<Notifications>()->setEnabled(true);
	this->getModule<HUD>()->setEnabled(true);
}

ModuleManager::~ModuleManager() {
	this->initialized = false;
	this->moduleList.clear();
}

void ModuleManager::onKeyUpdate(const int& key, const bool& isDown) {
	if (!this->initialized) return;
	for (const std::shared_ptr<Module>& mod : this->moduleList) {
		mod->onKeyUpdate(key, isDown);
	}
}

void ModuleManager::onImGuiRender(ImDrawList* d) {
	if (!this->initialized) return;
	for (const std::shared_ptr<Module>& mod : this->moduleList) {
		if (mod->isEnabled()) mod->onImGuiRender(d);
	}
}

void ModuleManager::onImGui3DRender(ImDrawList* d, glmatrixf* refdef) {
	if (!this->initialized) return;
	for (const std::shared_ptr<Module>& mod : this->moduleList) {
		if (mod->isEnabled()) mod->onImGui3DRender(d, refdef);
	}
}