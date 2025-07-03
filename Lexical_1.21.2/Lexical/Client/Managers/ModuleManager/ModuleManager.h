#pragma once
#include "Modules/ModuleBase/Module.h"

//COMBAT
#include "Modules/Category/Combat/AutoCrystal.h"
#include "Modules/Category/Combat/BowSpam.h"
#include "Modules/Category/Combat/Reach.h"
#include "Modules/Category/Combat/Criticals.h"
#include "Modules/Category/Combat/Hitbox.h"
#include "Modules/Category/Combat/Surround.h"
#include "Modules/Category/Combat/KillAura.h"

//MOVEMENT
#include "Modules/Category/Movement/AirJump.h"
#include "Modules/Category/Movement/AutoSprint.h"
#include "Modules/Category/Movement/ReverseStep.h"
#include "Modules/Category/Movement/Fly.h"
#include "Modules/Category/Movement/Jetpack.h"
#include "Modules/Category/Movement/Speed.h" 
#include "Modules/Category/Movement/NoClip.h"
#include "Modules/Category/Movement/ElytraFly.h"
#include "Modules/Category/Movement/NoSlowDown.h"
#include "Modules/Category/Movement/Phase.h"
#include "Modules/Category/Movement/Spider.h"
#include "Modules/Category/Movement/Velocity.h"
#include "Modules/Category/Movement/Step.h"

//RENDER
#include "Modules/Category/Render/BlockHighlight.h"
#include "Modules/Category/Render/BreakIndicators.h"
#include "Modules/Category/Render/CameraTweaks.h"
#include "Modules/Category/Render/Nametags.h"
#include "Modules/Category/Render/ChunkBorders.h"
#include "Modules/Category/Render/CustomFov.h"
#include "Modules/Category/Render/ESP.h"
#include "Modules/Category/Render/Fullbright.h"
#include "Modules/Category/Render/HurtColor.h"
#include "Modules/Category/Render/GlintColor.h"
#include "Modules/Category/Render/NoHurtCam.h"
#include "Modules/Category/Render/NoRender.h"
#include "Modules/Category/Render/Swing.h"
#include "Modules/Category/Render/Tracer.h"

//PLAYER
#include "Modules/Category/Player/AntiBot.h"
#include "Modules/Category/Player/AutoTool.h"
#include "Modules/Category/Player/BlockReach.h"
#include "Modules/Category/Player/FastUse.h"
#include "Modules/Category/Player/NoSwing.h"
#include "Modules/Category/Player/FastEat.h"
#include "Modules/Category/Player/Offhand.h"

//WORLD
#include "Modules/Category/World/AirPlace.h"

//MISC
#include "Modules/Category/Misc/Spammer.h"
#include "Modules/Category/Misc/Disabler.h"
#include "Modules/Category/Misc/PopCounter.h"
#include "Modules/Category/Misc/NoPacket.h"
#include "Modules/Category/Misc/Timer.h"

//CLIENT
#include "Modules/Category/Client/ClickGUI.h"
#include "Modules/Category/Client/Notifications.h"
#include "Modules/Category/Client/Hud.h"
#include "Modules/Category/Client/ArrayList.h"
#include "Modules/Category/Client/ColorsMod.h"
#include "Modules/Category/Client/CustomFont.h"


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

	static void onKeyUpdate(int key, bool isDown);
	static void onClientTick();
	static void onClientTick(Packet* packet);
	static void onNormalTick(LocalPlayer* localPlayer);
	static void onLevelTick(Level* level);
	static void onUpdateRotation(LocalPlayer* localPlayer);
	static void onSendPacket(Packet* packet);
	static void onD2DRender();
	static void onMCRender(MinecraftUIRenderContext* renderCtx);
	static void onLevelRender();
	static void onLoadConfig(void* conf);
	static void onSaveConfig(void* conf);
};
