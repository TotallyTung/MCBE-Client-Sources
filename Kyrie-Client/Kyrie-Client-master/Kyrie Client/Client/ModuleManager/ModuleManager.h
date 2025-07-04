#pragma once
#include "Modules/Module.h"

//Combat
//Misc
//Render
#include "Modules/Render/FullBright/FullBright.h"
#include "Modules/Render/Swing/Swing.h"
#include "Modules/Render/CustomFov/CustomFov.h"
//Movement
#include "Modules/Movement/Sprint/Sprint.h"
//Player
#include "Modules/Player/BlockReach/BlockReach.h"
//Client
#include "Modules/Client/Arraylist/Arraylist.h"
#include "Modules/Client/ClickGUI/ClickGui.h"
#include "Modules/Client/Colors/Colors.h"
#include "Modules/Client/CustomFont/CustomFont.h"
#include "Modules/Client/HUD/HUD.h"
#include "Modules/Client/Notifications/Notifications.h"
#include "Modules/Client/Unject/Unject.h"
//Unknow

class Client;

class ModuleManager {
private:
	std::vector<std::shared_ptr<Module>> moduleList;
	bool initialized = false;
public:
	Client* client = nullptr;
public:
	inline const std::vector<std::shared_ptr<Module>>& getModuleList() { return this->moduleList; }
	inline bool isInitialized() { return this->initialized; }
public:
	ModuleManager(Client* c);
	~ModuleManager();

	template <typename TRet>
	TRet* getModule() {
		if (!isInitialized())
			return nullptr;
		for (std::shared_ptr<Module> pMod : moduleList) {
			if (auto pRet = dynamic_cast<typename std::remove_pointer<TRet>::type*>(pMod.get())) {

				return pRet;
			}
		}
		return nullptr;
	};

	void onKeyUpdate(const int& key, const bool& isDown);
	void onImGuiRender(ImDrawList* d);
	void onImGui3DRender(ImDrawList* d, glmatrixf* refdef);
};
