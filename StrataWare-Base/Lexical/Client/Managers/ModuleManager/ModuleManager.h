#pragma once
#include "Modules/ModuleBase/Module.h"
#include "Modules/Category/Client/ClickGUI.h"
#include "Modules/Category/Client/CustomFont.h"
#include "Modules/Category/Client/Hud.h"
#include "Modules/Category/Combat/Killaura.hpp"
#include "Modules/Category/Combat/AutoCrystal.h" 
#include "Modules/Category/Combat/Hitbox.h"
#include "Modules/Category/Combat/BowSpam.h"
#include "Modules/Category/Combat/Reach.h"
#include "Modules/Category/Client/D2DHud.h"
#include "Modules/Category/Combat/AutoAnvil.h"
#include "Modules/Category/Combat/AutoCLicker.h"
class ModuleManager {
public:
	static inline std::vector<Module*> moduleList;

	static void init();
	static void shutdown();

	template <typename TRet>
	static TRet* getModule() {
		for (Module* mod : moduleList) {
			TRet* result = dynamic_cast<TRet*>(mod);
			if (result == nullptr)
				continue;
			return result;
		}
		return nullptr;
	}
	static void onUpdateRotation(LocalPlayer* localPlayer);
	static void onKeyUpdate(int key, bool isDown);
	static void onPlayerTick();
	static void onPlayerTick(Packet* packet);
	static void onNormalTick(LocalPlayer* localPlayer);
	static void onWorldTick(Level* level);
	static bool onSendPacket(Packet* packet);
	static bool onSendDelayedPacket(Packet* packet);
	static void onD2DRender();
	static void onMCRender(MinecraftUIRenderContext* renderCtx);
	static void onLevelRender();
	static void onLoadConfig(void* conf);
	static void onSaveConfig(void* conf);
	static void onMatrixRender(glm::mat4* matrix);
};
