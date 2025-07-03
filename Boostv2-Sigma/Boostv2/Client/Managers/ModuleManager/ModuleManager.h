#pragma once
#include "Modules/ModuleBase/Module.h"

//COMBAT
#include "Modules/Category/Combat/AutoCrystal.h"
#include "Modules/Category/Combat/BowSpam.h"
#include "Modules/Category/Combat/Reach.h"
#include "Modules/Category/Combat/Criticals.h"
#include "Modules/Category/Combat/Hitbox.h"
#include "Modules/Category/Combat/Surround.h"
#include "Modules/Category/Combat/TriggerBot.h"
#include "Modules/Category/Combat/AutoTrap.h"
#include "Modules/Category/Combat/AutoClicker.h"
#include "Modules/Category/Combat/AutoAnvil.h"
#include "Modules/Category/Combat/KillAura.h"
#include "Modules/Category/Combat/popcounter.h"
#include "Modules/Category/Combat/GodMode.h"
#include "Modules/Category/Combat/DamageText.h"
#include "Modules/Category/Combat/Switcher.h"
#include "Modules/Category/Combat/KillauraReach.h"

//MOVEMENT
#include "Modules/Category/Movement/AirJump.h"
#include "Modules/Category/Movement/AutoSprint.h"
#include "Modules/Category/Movement/ReverseStep.h"
#include "Modules/Category/Movement/Fly.h"
#include "Modules/Category/Movement/Jesus.h"
#include "Modules/Category/Movement/Jetpack.h"
#include "Modules/Category/Movement/Speed.h"
#include "Modules/Category/Movement/NoClip.h"
#include "Modules/Category/Movement/ElytraFly.h"
#include "Modules/Category/Movement/NoSlowDown.h"
#include "Modules/Category/Movement/Phase.h"
#include "Modules/Category/Movement/Spider.h"
#include "Modules/Category/Movement/Velocity.h"
#include "Modules/Category/Movement/AutoJump.h"
#include "Modules/Category/Movement/Step.h"

//RENDER
#include "Modules/Category/Render/Arraylist.h"
#include "Modules/Category/Render/ClickTP.h"
#include "Modules/Category/Render/BlockHighlight.h"
#include "Modules/Category/Render/BreakIndicators.h"
#include "Modules/Category/Render/CameraTweaks.h"
#include "Modules/Category/Render/Nametags.h"
#include "Modules/Category/Render/ViewModel.h"
#include "Modules/Category/Render/ChunkBorders.h"
#include "Modules/Category/Render/CustomSky.h"
#include "Modules/Category/Render/CustomFov.h"
#include "Modules/Category/Render/ESP.h"
#include "Modules/Category/Render/ItemESP.h"
#include "Modules/Category/Render/Fullbright.h"
#include "Modules/Category/Render/UIBlur.h"
#include "Modules/Category/Render/HurtColor.h"
#include "Modules/Category/Render/GlintColor.h"
#include "Modules/Category/Render/HUD.h"
#include "Modules/Category/Render/Trajectories.h"
#include "Modules/Category/Render/TargetHUD.h"
#include "Modules/Category/Render/StorageESP.h"
#include "Modules/Category/Render/PlayEmote.h"
#include "Modules/Category/Render/NoHurtCam.h"
#include "Modules/Category/Render/PortalESP.h"
#include "Modules/Category/Render/NoRender.h"
#include "Modules/Category/Render/Swing.h"
#include "Modules/Category/Render/Tracer.h"

//PLAYER
#include "Modules/Category/Player/AutoAFKMine.h"
#include "Modules/Category/Player/AutoOreBreaker.h"
#include "Modules/Category/Player/AutoMine.h"
#include "Modules/Category/Player/AntiBot.h"
#include "Modules/Category/Player/AutoTool.h"
#include "Modules/Category/Player/NoFriends.h"
#include "Modules/Category/Player/BlockReach.h"
#include "Modules/Category/Player/EnemyTp.h"
#include "Modules/Category/Player/Playerlist.h"
#include "Modules/Category/Player/MidClick.h"
#include "Modules/Category/Player/PacketMine.h"
#include "Modules/Category/Player/FastUse.h"
#include "Modules/Category/Player/NoSwing.h"
#include "Modules/Category/Player/Crasher.h"
#include "Modules/Category/Player/FastEat.h"
#include "Modules/Category/Player/Offhand.h"

//WORLD
#include "Modules/Category/World/Freecam.h"
#include "Modules/Category/World/AirPlace.h"
#include "Modules/Category/World/AutoLeave.h"
#include "Modules/Category/World/NoFall.h"
#include "Modules/Category/World/AutoEZ.h"
#include "Modules/Category/World/Surface.h"
#include "Modules/Category/World/ChestTP.h"
#include "Modules/Category/World/WaterTP.h"

//MISC
#include "Modules/Category/Misc/Spammer.h"
#include "Modules/Category/Misc/Disabler.h"
#include "Modules/Category/Misc/PlayerJoin.h"
#include "Modules/Category/Misc/NoPacket.h"
#include "Modules/Category/Misc/Timer.h"

//CLIENT
#include "Modules/Category/Client/ToggleSounds.h"
#include "Modules/Category/Client/Colors.h"
#include "Modules/Category/Client/ClickGUI.h"
#include "Modules/Category/Client/Notifications.h"
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
	static void onNormalTick(LocalPlayer* localPlayer);
	static void onLevelTick(Level* level);
	static void onMatrixRender(glm::mat4* matrix);
	static void onUpdateRotation(LocalPlayer* localPlayer);
	static void onSendPacket(Packet* packet);
	static void onD2DRender();
	static void onMCRender(MinecraftUIRenderContext* renderCtx);
	static void onLevelRender();
	static void onLoadConfig(void* conf);
	static void onSaveConfig(void* conf);
};
