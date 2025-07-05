#pragma once

#include <typeinfo>
#include <vector>
#include <optional>
#include <memory>
#include <mutex>
#include <shared_mutex>

#include "../../Memory/GameData.h"

#ifdef _BETA

#endif

#include "Modules/AutoCity.h"
//#include "Modules/PacketMine.h"
#include "Modules/AntiCrystal.h"
#include "Modules/APVPDisabler.h"
#include "Modules/RobotHolefiller.h"
#include "Modules/CityESP.h"
#include "Modules/DeathCoords.h"
#include "Modules/RenSurround.h"
#include "Modules/SilentLook.h"
#include "Modules/StrafeSpeed.h"
#include "Modules/Burrow.h"
#include "Modules/PvPResources.h"
#include "Modules/LavaPlacer.h"
#include "Modules/BlockEsp.h"
#include "Modules/AnvilAura.h"
#include "Modules/BowSpam.h"
#include "Modules/LiquidMove.h"
#include "Modules/ReverseStep.h"

#include "Modules/CrystalAuraWTA.h"
#include "Modules/FastXPtwo.h"
#include "Modules/BPGRewrite.h"
#include "Modules/CrystalUtilsJTWD.h"  // in progress
//#include "Modules/ManualCrystal.h"// fixed
#include "Modules/ExamModule.h"
#include "Modules/NoClip.h"		  // works
#include "Modules/AnchorAura.h"	  // broken
#include "Modules/AutoEz.h"		  // retarded
#include "Modules/surround.h"     // fixed
#include "Modules/AutoTrap.h"	  // fixed
#include "Modules/HoleESP.h"      // fixed
#include "Modules/HoleFiller.h"	  // fixed
#include "Modules//FastExp.h"

#include "Modules/Aimbot.h"
#include "Modules/AirJump.h"
#include "Modules/AirSwim.h"
#include "Modules/AntiBot.h"
#include "Modules/AutoArmor.h"
#include "Modules/AutoClicker.h"
#include "Modules/AutoSneak.h"
#include "Modules/AutoSprint.h"
#include "Modules/AutoTotem.h"
#include "Modules/AutoTotem2.h"
#include "Modules/Fucker.h"
#include "Modules/Bhop.h"
#include "Modules/Blink.h"
#include "Modules/BowAimbot.h"
#include "Modules/AntiVoid.h"
#include "Modules/ChestAura.h"
//#include "Modules/StorageESP.h"
#include "Modules/ChestStealer.h"
#include "Modules/ClickGuiMod.h"
#include "Modules/Teleport.h"
#include "Modules/Crasher.h"
#include "Modules/Criticals.h"
#include "Modules/Placer.h"
#include "Modules/CrystalAura.h"
#include "Modules/CrystalAuraOW.h"
#include "Modules/Disabler.h"
#include "Modules/Derp.h"
#include "Modules/ESP.h"
#include "Modules/EditionFaker.h"
#include "Modules/FastEat.h"
#include "Modules/FastLadder.h"
#include "Modules/Fly.h"
#include "Modules/Freecam.h"
#include "Modules/FullBright.h"
#include "Modules/Glide.h"
#include "Modules/HighJump.h"
#include "Modules/Hitbox.h"
#include "Modules/HudModule.h"
#include "Modules/ExtendedBlockReach.h"
#include "Modules/InfiniteAura.h"
#include "Modules/InstaBreak.h"
#include "Modules/InventoryCleaner.h"
#include "Modules/InventoryMove.h"
#include "Modules/Jesus.h"
#include "Modules/Jetpack.h"
#include "Modules/Killaura.h"
#include "Modules/TPAura.h"
#include "Modules/MidClick.h"
#include "Modules/Module.h"
#include "Modules/NameTags.h"
#include "Modules/NoFall.h"
#include "Modules/NoFriends.h"
#include "Modules/Velocity.h"
#include "Modules/NoPacket.h"
#include "Modules/NoSlowDown.h"
#include "Modules/NoWeb.h"
#include "Modules/Nuker.h"
#include "Modules/Phase.h"
#include "Modules/RainbowSky.h"
#include "Modules/Reach.h"
#include "Modules/Scaffold.h"
#include "Modules/Spammer.h"
#include "Modules/Speed.h"
#include "Modules/StackableItem.h"
#include "Modules/Step.h"
#include "Modules/TimeChanger.h"
#include "Modules/Timer.h"
#include "Modules/Tower.h"
#include "Modules/Tracer.h"
#include "Modules/TriggerBot.h"
#include "Modules/Xray.h"
#include "Modules/NightMode.h"
#include "Modules/NoSwing.h"
#include "Modules/CubeGlide.h"
#include "Modules/AirStuck.h"
#include "Modules/Zoom.h"
#include "Modules/Teams.h"
#include "Modules/Nbt.h"
#include "Modules/Godmode.h"
#include "Modules/Freelook.h"
#include "Modules/NoHurtcam.h"
#include "Modules/AntiImmobile.h"
#include "Modules/FollowPathModule.h"
#include "Modules/Waypoints.h"
#include "Modules/Spider.h"
#include "Modules/Compass.h"
#include "Modules/Radar.h"
#include "Modules/VanillaPlus.h"
#include "Modules/ViewModel.h"
#include "Modules/Twerk.h"
#include "Modules/FontChanger.h"
#include "Modules/Watermark.h"
#include "Modules/Theme.h"
#include "Modules/Chat.h"
#include "Modules/Welcome.h"
#include "Modules/Switcher.h"
#include "Modules/FlyBypass.h"
#include "Modules/autowalk.h"
#include "Modules/noentityrender.h"
#include "Modules/boatfly.h"
#include "Modules/boatspeed.h"
#include "Modules/Verification.h"
#include "Modules/arraylist.h"
#include "Modules/targethud.h"
#include "Modules/invview.h"
#include "Modules/watermark2.h"
#include "Modules/AutoLog.h"
#include "Modules/silentswitch.h"
#include "Modules/crystalauranukkit.h"
#include "Modules/playerlist.h"
#include "Modules/CpsAura.h"
#include "Modules/AutoHotbar.h"
#include "Modules/ElytraFly.h"
#include "Modules/AntiSurround.h"
#include "Modules/2b2eSpeed.h"
#ifdef _DEBUG 
#endif


class ModuleManager {
private:
	GameData* gameData;
	std::vector<std::shared_ptr<IModule>> moduleList;
	bool initialized = false;
	std::shared_mutex moduleListMutex;

public:
	~ModuleManager();
	ModuleManager(GameData* gameData);
	void initModules();
	void disable();
	void onLoadConfig(void* conf);
	void onSaveConfig(void* conf);
	void onTick(C_GameMode* gameMode);
	void onAttack(C_Entity* attackedEnt);
	void onWorldTick(C_GameMode* gameMode);
	void onKeyUpdate(int key, bool isDown);
	void onPreRender(C_MinecraftUIRenderContext* renderCtx);
	void onPostRender(C_MinecraftUIRenderContext* renderCtx);
	void onLevelRender();
	void onMove(C_MoveInputHandler* handler);
	void onPlayerTick(C_Player* plr);
	void onSendPacket(C_Packet*);

	std::shared_lock<std::shared_mutex> lockModuleList() { return std::shared_lock(moduleListMutex); }
	std::unique_lock<std::shared_mutex> lockModuleListExclusive() { return std::unique_lock(moduleListMutex); }
	
	std::shared_mutex* getModuleListLock() { return &moduleListMutex; }

	bool isInitialized() { return initialized; };
	std::vector<std::shared_ptr<IModule>>* getModuleList();

	int getModuleCount();
	int getEnabledModuleCount();

	/*
	 *	Use as follows: 
	 *		IModule* mod = moduleMgr.getModule<NoKnockBack>(); 
	 *	Check for nullptr directly after that call, as Hooks::init is called before ModuleManager::initModules !	
	 */
	template <typename TRet>
	TRet* getModule() {
		if (!isInitialized())
			return nullptr;
		auto lock = lockModuleList();
		for (auto pMod : moduleList) {
			if (auto pRet = dynamic_cast<typename std::remove_pointer<TRet>::type*>(pMod.get())){
				
				return pRet;
			}
		}
		return nullptr;
	};

	// Dont Use this functions unless you absolutely need to. The function above this one works in 99% of cases
	std::optional<std::shared_ptr<IModule>> getModuleByName(const std::string name) {
		if (!isInitialized())
			return nullptr;
		std::string nameCopy = name;
		std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(), ::tolower);
		
		auto lock = lockModuleList();
		for (std::vector<std::shared_ptr<IModule>>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
			std::shared_ptr<IModule> mod = *it;
			std::string modNameCopy = mod->getRawModuleName();
			std::transform(modNameCopy.begin(), modNameCopy.end(), modNameCopy.begin(), ::tolower);
			if (modNameCopy == nameCopy)
				return std::optional<std::shared_ptr<IModule>>(mod);
		}
		return std::optional<std::shared_ptr<IModule>>();
	}
};

extern ModuleManager* moduleMgr;
