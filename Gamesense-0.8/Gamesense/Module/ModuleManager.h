#pragma once

#include <typeinfo>
#include <vector>
#include <optional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>

#include "../../Memory/GameData.h"

// Menu
#include "Modules/ConfigManagerMod.h"
#include "Modules/ClickGuiMod.h"
#include "Modules/DebugMenu.h"

// Combat
#include "Modules/Combat/AutoCrystalRewrite.h"
#include "Modules/Combat/TNTBomber.h"
#include "Modules/Combat/BedAuraRewrite.h"
#include "Modules/Combat/TPAura.h"
#include "Modules/Combat/AutoCrystal.h"
#include "Modules/Combat/BetterBow.h"
#include "Modules/Combat/AutoObsidian.h"
#include "Modules/Combat/AntiCrystal.h"
#include "Modules/Combat/TriggerBot.h"
#include "Modules/Combat/Surround.h"
#include "Modules/Combat/AutoTrap.h"
#include "Modules/Combat/Killaura.h"
#include "Modules/Combat/AntiBot.h"
#include "Modules/Combat/Reach.h"

// Visual
#include "Modules/Visual\HUD.h"
#include "Modules/Visual\BreakHighlight.h"
#include "Modules/Visual\ViewModel.h"
#include "Modules/Visual\TargetVisualize.h"
#include "Modules/Visual\SearchBlocks.h"
#include "Modules/Visual\SwingSpeed.h"
#include "Modules/Visual/Notifications.h"
#include "Modules/Visual/BlockOutline.h"
#include "Modules/Visual/PlayerList2.h"
#include "Modules/Visual/Animations.h"
#include "Modules/Visual/ArrayList.h"
#include "Modules/Visual/Interface.h"
#include "Modules/Visual/TargetHUD.h"
#include "Modules/Visual/Watermark.h"
#include "Modules/Visual/HoleEsp.h"
#include "Modules/Visual/Nametags.h"
#include "Modules/Visual/NoRender.h"
#include "Modules/Visual/Camera.h"
#include "Modules/Visual/Zoom.h"
#include "Modules/Visual/ESP.h"

// Movement
#include "Modules/Movement/ReverseStep.h"
#include "Modules/Movement/Spider.h"
#include "Modules/Movement/AutoClip.h"
#include "Modules/Movement/PacketFly.h"
#include "Modules/Movement/NoClipGlide.h"
#include "Modules/Movement/Anchor.h"
#include "Modules/Movement/2b2espeed.h"
// #include "Modules/Movement/ElytraFly.h"
#include "Modules/Movement/Velocity.h"
#include "Modules/Movement/FastStop.h"
#include "Modules/Movement/Jetpack.h"
#include "Modules/Movement/InvMove.h"
#include "Modules/Movement/NoClip.h"
#include "Modules/Movement/Flight.h"
#include "Modules/Movement/Sprint.h"
#include "Modules/Movement/NoSlowDown.h"
#include "Modules/Movement/Speed.h"
#include "Modules/Movement/Sneak.h"
#include "Modules/Movement/Jesus.h"
// #include "Modules/Unused/Glide.h"
#include "Modules/Movement/Step.h"

// Player
#include "Modules/Player/ChestStealer.h"
#include "Modules/Player/GravityAura.h"
#include "Modules/Player/AutoHoleFill.h"
#include "Modules/Player/GhostHand.h"
#include "Modules/Player/AutoSwitch.h"
#include "Modules/Player/BasePlace.h"
#include "Modules/Player/Burrow.h"
#include "Modules/Player/AutoTool.h"
#include "Modules/Player/AutoHotbar.h"
// #include "Modules/Player/InvManager.h"
#include "Modules/Unused/FastPlace.h"
#include "Modules/Player/AutoTotem.h"
#include "Modules/Player/AutoArmor.h"
#include "Modules/Player/Scaffold.h"
#include "Modules/Player/Freecam.h"
#include "Modules/Unused/FastEat.h"
#include "Modules/Player/Blink.h"
#include "Modules/Player/PacketMine.h"
#include "Modules/Player/Timer.h"

// Exploit
#include "Modules/Exploit/AntiDesync.h"
#include "Modules/Exploit/AutoKit.h"
#include "Modules/Exploit/PacketXP.h"
#include "Modules/Exploit/StashFinder.h"
#include "Modules/Exploit/OffhandAllow.h"
#include "Modules/Exploit/AutoDupe.h"
#include "Modules/Exploit/NoHunger.h"
#include "Modules/Exploit/AutoRespawn.h"
#include "Modules/Exploit/NoEntityTrace.h"
#include "Modules/Exploit/FastUse.h"
#include "Modules/Exploit/Phase.h"

// Other
#include "Modules/Other/ClientSetting.h"
#include "Modules/Other/ToggleSounds.h"
#include "Modules/Other/TestModule.h"
#include "Modules/Other/ChatSuffix.h"
// #include "Modules/Other/Breaker.h"
#include "Modules/Other/Spammer.h"
#include "Modules/Other/Derp.h"
#include "Modules/Other/MCF.h"

#include "Modules/Module.h"

class ModuleManager
{
private:
	GameData *gameData;
	std::vector<std::shared_ptr<IModule>> moduleList;
	bool initialized = false;
	std::shared_mutex moduleListMutex;

public:
	~ModuleManager();
	ModuleManager(GameData *gameData);
	void initModules();
	void disable();
	void onLoadConfig(void *conf);
	void onSaveConfig(void *conf);
	void onLoadSettings(void *conf);
	void onSaveSettings(void *conf);
	void onTick(C_GameMode *gameMode);
	void onPreTick(C_GameMode* gameMode);
	void onAttack(C_Entity *attackedEnt);
	void onPlayerTick(C_Player *plr);
	void onWorldTick(C_GameMode *gameMode);
	void onKeyUpdate(int key, bool isDown);
	void onPreRender(C_MinecraftUIRenderContext *renderCtx);
	void onPostRender(C_MinecraftUIRenderContext *renderCtx);
	void onImGuiRender(ImDrawList* d);
	void onLevelRender();
	void onMove(C_MoveInputHandler *handler);
	void onSendPacket(C_Packet *);

	std::shared_lock<std::shared_mutex> lockModuleList() { return std::shared_lock(this->moduleListMutex); }
	std::unique_lock<std::shared_mutex> lockModuleListExclusive() { return std::unique_lock(this->moduleListMutex); }

	std::shared_mutex *getModuleListLock() { return &this->moduleListMutex; }

	bool isInitialized() { return initialized; };
	std::vector<std::shared_ptr<IModule>> *getModuleList();

	int getModuleCount();
	int getEnabledModuleCount();

	/*\
	 *	Use as follows: well no shit
	 *	IModule* mod = moduleMgr->getModule<NoKnockBack>();
	 *	Check for nullptr directly after that call, as Hooks::init is called before ModuleManager::initModules !
	\*/

	template <typename TRet>
	TRet *getModule()
	{
		if (!isInitialized())
			return nullptr;
		auto lock = this->lockModuleList();
		for (auto pMod : moduleList)
		{
			if (auto pRet = dynamic_cast<typename std::remove_pointer<TRet>::type *>(pMod.get()))
			{

				return pRet;
			}
		}
		return nullptr;
	};

	// Dont Use this functions unless you absolutely need to. The function above this one works in 99% of cases
	std::optional<std::shared_ptr<IModule>> getModuleByName(const std::string name)
	{
		if (!isInitialized())
			return nullptr;
		std::string nameCopy = name;
		std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(), ::tolower);

		auto lock = this->lockModuleList();
		for (std::vector<std::shared_ptr<IModule>>::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it)
		{
			std::shared_ptr<IModule> mod = *it;
			std::string modNameCopy = mod->getRawModuleName();
			std::transform(modNameCopy.begin(), modNameCopy.end(), modNameCopy.begin(), ::tolower);
			if (modNameCopy == nameCopy)
				return std::optional<std::shared_ptr<IModule>>(mod);
		}
		return std::optional<std::shared_ptr<IModule>>();
	}
};

extern ModuleManager *moduleMgr;