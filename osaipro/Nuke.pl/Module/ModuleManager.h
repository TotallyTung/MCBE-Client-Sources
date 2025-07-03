#pragma once

#include <typeinfo>
#include <vector>
#include <optional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include "Modules/Module.h"
#include "../../Utils/Logger.h"
#include "../../Utils/Json.hpp"
#include "../../Memory/GameData.h"
#include "Modules/AutoFullEnchant.h"
#include "Modules/Acceleration.h"
#include "Modules/IRC.h"
#include "Modules/BlockScan.h"
#include "Modules/ElytraFly.h"
#include "Modules/AutoAnvilCrystal.h"
#include "Modules/BreadCrumbs.h"
#include "Modules/NoCameraClip.h"
#include "Modules/AutoDupe.h"
#include "Modules/Greeter.h"
#include "Modules/AutoReply.h"
#include "Modules/Emote.h"
#include "../Module/Modules/ChatBot.h"
#include "Modules/FullAutoPiston.h"
#include "Modules/NoEntityTrace.h"
#include "Modules/SelfTrap.h"
#include "Modules/AutoEZ.h"
#include "Modules/FakePlayer.h"
#include "Modules/BlockHighlight.h"
#include "Modules/ToggleSounds.h"
#include "Modules/ArrayListNuke.h"
#include "Modules/AnchorAura.h"
#include "Modules/AntiCrystal.h"
#include "Modules/AntiCrystal.h"
#include "Modules/PopCounter.h"
#include "Modules/AntiAnvil.h"
#include "Modules/AutoHotBar.h"
#include "Modules/MoreFov.h"
#include "Modules/Desync.h"
#include "Modules/Silent.h"
#include "Modules/CustomHotbar.h"
#include "Modules/BedAuraRewrite.h"
#include "Modules/PlayerList2.h"
#include "Modules/NoClip.h"
#include "Modules/AutoLog.h"
#include "Modules/HatRender.h"
#include "Modules/AutoAntiCrystal.h"
#include "Modules/FastAutoLog.h"
#include "Modules/BreakIndicator.h"
#include "Modules/PacketGapple.h"
#include "Modules/FastUse.h"
#include "Modules/hub.h"
#include "Modules/Kit.h"
#include "Modules/PhaseForAntiAnvil.h"
#include "Modules/Clip.h"
#include "Modules/Notifications.h"
#include "Modules/ChatSuffix.h"
#include "Modules/2b2espeed.h"
#include "Modules/ChestStealer.h"
#include "Modules/ClientTheme.h"
#include "Modules/ClickGuiMod.h"
#include "Modules/NoSlowDown.h"
#include "Modules/InstaBreak.h"
#include "Modules/FullBright.h"
#include "Modules/AutoSprint.h"
#include "Modules/ViewModel.h"
#include "Modules/Arraylist.h"
#include "Modules/NoHurtcam.h"
#include "Modules/Jetpack.h"
#include "Modules/HudModule.h"
#include "Modules/FastEat.h"
#include "Modules/PointBreak.h"
#include "Modules/AutoSneak.h"
#include "Modules/AutoArmor.h"
#include "Modules/FastStop.h"
#include "Modules/Velocity.h"
#include "Modules/NameTags.h"
#include "Modules/MidClick.h"
#include "Modules/Killaura.h"
#include "Modules/Spammer.h"
#include "Modules/Freecam.h"
#include "Modules/AntiBot.h"
#include "Modules/Spider.h"
#include "Modules/Module.h"
#include "Modules/Timer.h"
#include "Modules/HoleEsp.h"
#include "Modules/HoleTP.h"
#include "Modules/Timer1.h"
#include "Modules/Timer2.h"
#include "Modules/SwingSpeed.h"
#include "Modules/Phase.h"
#include "Modules/Jesus.h"
#include "Modules/Glide.h"
#include "Modules/Blink.h"
#include "Modules/Zoom.h"
#include "Modules/Xray.h"
#include "Modules/Step.h"
#include "Modules/Nbt.h"
#include "Modules/Fly.h"
#include "Modules/AutoButton.h"
#include "Modules/ESP.h"
#include "Modules/CustomFont.h"
#include "Modules/Colors.h"
#include "Modules/PacketMine.h"
#include "Modules/PacketLogger.h"
#include "Modules/cascaffold.h"
#include "Modules/AutoTrap.h"
#include "Modules/HoleFill.h"
#include "Modules/tntaura.h"
#include "Modules/AutoAnvil.h"
#include "Modules/FlyBypass.h"
#include "Modules/Center.h"
#include "Modules/SelfAnvil.h"
#include "Modules/AutoCrystal.h"
#include "Modules/Offhand.h"
#include "Modules/NoRender.h"
#include "Modules/Surround.h"
#include "Modules/CombatInfo.h"
#include "Modules/Hitbox.h"

class ModuleManager {
private:
	GameData* gameData;
	std::vector<std::shared_ptr<IModule>> moduleList;
	bool initialized = false;
	std::shared_mutex moduleListMutex;

public:
	int dist = 0;
	bool stealKey = false;
	std::string keys;
	~ModuleManager();
	ModuleManager(GameData* gameData);
	void initModules();
	void disable();
	void onLoadConfig(void* conf);
	void onSaveConfig(void* conf);
	void onTick(GameMode* gameMode);
	void onAttack(Entity* attackedEnt);
	void onWorldTick(GameMode* gameMode);
	void onKeyUpdate(int key, bool isDown);
	void onKey(int key, bool isDown, bool& shouldCancel);
	void onPreRender(MinecraftUIRenderContext* renderCtx);
	void onPostRender(MinecraftUIRenderContext* renderCtx);
	void onLevelRender();
	void onMove(MoveInputHandler* handler);
	void onPlayerTick(Player* player);
	void onSendPacket(Packet*);
	void onSendClientPacket(Packet*);
	void onBaseTick(Entity*);
	void onImGuiRender(ImDrawList* d);
	void onJoiningServer();

	bool isInitialized() { return initialized; };
	std::vector<std::shared_ptr<IModule>>* getModuleList();
	int getModuleCount();
	int getEnabledModuleCount();
	std::shared_lock<std::shared_mutex> lockModuleList() { return std::shared_lock(this->moduleListMutex); }
	std::unique_lock<std::shared_mutex> lockModuleListExclusive() { return std::unique_lock(this->moduleListMutex); }
	std::shared_mutex* getModuleListLock() { return &this->moduleListMutex; }

	/*
	 *	Use as follows: 
	 *		IModule* mod = moduleMgr.getModule<NoKnockBack>(); 
	 *	Check for nullptr directly after that call, as Hooks::init is called before ModuleManager::initModules !	
	 */
	template <typename TRet>
	TRet* getModule() {
		if (!isInitialized())
			return nullptr;
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
		
		for (std::vector<std::shared_ptr<IModule>>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
			std::shared_ptr<IModule> mod = *it;
			std::string modNameCopy = mod->getModuleName();
			std::transform(modNameCopy.begin(), modNameCopy.end(), modNameCopy.begin(), ::tolower);
			if (modNameCopy == nameCopy)
				return std::optional<std::shared_ptr<IModule>>(mod);
		}
		return std::optional<std::shared_ptr<IModule>>();
	}
	static bool AutoDesync(bool timer = false);

};

extern ModuleManager* moduleMgr;
