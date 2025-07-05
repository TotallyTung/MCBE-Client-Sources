#include "ModuleManager.h"
#include "../../Utils/Logger.h"
#include "../../Utils/Json.hpp"

using json = nlohmann::json;

ModuleManager::ModuleManager(GameData* gameData) {
	gameData = gameData;
}

ModuleManager::~ModuleManager() {
	initialized = false;
	auto lock = lockModuleListExclusive();
	moduleList.clear();
}

void ModuleManager::initModules() {
	logF("Initializing modules");
	{
		auto lock = lockModuleListExclusive();

#ifdef _BETA
	
#endif  // BETA

		moduleList.push_back(std::shared_ptr<IModule>(new AutoCity()));
		//moduleList.push_back(std::shared_ptr<IModule>(new PacketMine()));
		moduleList.push_back(std::shared_ptr<IModule>(new AntiCrystal()));
		moduleList.push_back(std::shared_ptr<IModule>(new APVPDisabler()));
		moduleList.push_back(std::shared_ptr<IModule>(new RobotHolefiller()));
		moduleList.push_back(std::shared_ptr<IModule>(new CityESP()));
		moduleList.push_back(std::shared_ptr<IModule>(new DeathCoords()));
		moduleList.push_back(std::shared_ptr<IModule>(new SilentLook()));
		moduleList.push_back(std::shared_ptr<IModule>(new RenSurround()));
		moduleList.push_back(std::shared_ptr<IModule>(new StrafeSpeed()));
		moduleList.push_back(std::shared_ptr<IModule>(new Burrow()));
		moduleList.push_back(std::shared_ptr<IModule>(new pvpResources()));
		moduleList.push_back(std::shared_ptr<IModule>(new LavaPlacer()));
		moduleList.push_back(std::shared_ptr<IModule>(new BlockEsp()));
		moduleList.push_back(std::shared_ptr<IModule>(new AnvilAura()));
		moduleList.push_back(std::shared_ptr<IModule>(new BowSpam()));
		moduleList.push_back(std::shared_ptr<IModule>(new LiquidMove()));
		moduleList.push_back(std::shared_ptr<IModule>(new ReverseStep()));

		moduleList.push_back(std::shared_ptr<IModule>(new CrystalAuraWTA()));
		moduleList.push_back(std::shared_ptr<IModule>(new FastXPtwo()));
		moduleList.push_back(std::shared_ptr<IModule>(new BPGRewrite()));
		moduleList.push_back(std::shared_ptr<IModule>(new FastXP()));
		moduleList.push_back(std::shared_ptr<IModule>(new CrystalPlace()));
		moduleList.push_back(std::shared_ptr<IModule>(new CrystalBreak()));
		//moduleList.push_back(std::shared_ptr<IModule>(new ManualCrystal()));
		moduleList.push_back(std::shared_ptr<IModule>(new ExamModule()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoClip()));
		moduleList.push_back(std::shared_ptr<IModule>(new AnchorAura()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoEZ()));
		moduleList.push_back(std::shared_ptr<IModule>(new Surround()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoTrap()));
		moduleList.push_back(std::shared_ptr<IModule>(new HoleEsp()));
		moduleList.push_back(std::shared_ptr<IModule>(new HoleFiller()));

		moduleList.push_back(std::shared_ptr<IModule>(new HudModule()));
		moduleList.push_back(std::shared_ptr<IModule>(new Killaura()));
		moduleList.push_back(std::shared_ptr<IModule>(new TPAura()));
		moduleList.push_back(std::shared_ptr<IModule>(new ESP()));
		moduleList.push_back(std::shared_ptr<IModule>(new Jetpack()));
		moduleList.push_back(std::shared_ptr<IModule>(new Aimbot()));
		moduleList.push_back(std::shared_ptr<IModule>(new TriggerBot()));
		//moduleList.push_back(std::shared_ptr<IModule>(new StorageESP()));
		moduleList.push_back(std::shared_ptr<IModule>(new Velocity()));
		moduleList.push_back(std::shared_ptr<IModule>(new RainbowSky()));
		moduleList.push_back(std::shared_ptr<IModule>(new AirJump()));
		moduleList.push_back(std::shared_ptr<IModule>(new Step()));
		moduleList.push_back(std::shared_ptr<IModule>(new Glide()));
		moduleList.push_back(std::shared_ptr<IModule>(new EditionFaker()));
		moduleList.push_back(std::shared_ptr<IModule>(new Phase()));
		moduleList.push_back(std::shared_ptr<IModule>(new Freecam()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoSprint()));
		moduleList.push_back(std::shared_ptr<IModule>(new Tracer()));
		moduleList.push_back(std::shared_ptr<IModule>(new BowAimbot()));
		moduleList.push_back(std::shared_ptr<IModule>(new Scaffold()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoFall()));
		moduleList.push_back(std::shared_ptr<IModule>(new Blink()));
		moduleList.push_back(std::shared_ptr<IModule>(new Nuker()));
		moduleList.push_back(std::shared_ptr<IModule>(new InstaBreak()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoPacket()));
		moduleList.push_back(std::shared_ptr<IModule>(new Speed()));
		moduleList.push_back(std::shared_ptr<IModule>(new Xray()));
		moduleList.push_back(std::shared_ptr<IModule>(new Fucker()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoSlowDown()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoTotem()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoTotem2()));
		moduleList.push_back(std::shared_ptr<IModule>(new InfiniteAura()));
		moduleList.push_back(std::shared_ptr<IModule>(new ClickGuiMod()));
		moduleList.push_back(std::shared_ptr<IModule>(new ChestStealer()));
		moduleList.push_back(std::shared_ptr<IModule>(new Fly()));
		moduleList.push_back(std::shared_ptr<IModule>(new Hitbox()));
		moduleList.push_back(std::shared_ptr<IModule>(new Reach()));
		moduleList.push_back(std::shared_ptr<IModule>(new FullBright()));
		moduleList.push_back(std::shared_ptr<IModule>(new AirSwim()));
		moduleList.push_back(std::shared_ptr<IModule>(new InventoryMove()));
		moduleList.push_back(std::shared_ptr<IModule>(new HighJump()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoWeb()));
		moduleList.push_back(std::shared_ptr<IModule>(new FastLadder()));
		moduleList.push_back(std::shared_ptr<IModule>(new StackableItem()));
		moduleList.push_back(std::shared_ptr<IModule>(new FastEat()));
		moduleList.push_back(std::shared_ptr<IModule>(new Jesus()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoArmor()));
		moduleList.push_back(std::shared_ptr<IModule>(new ExtendedBlockReach()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoClicker()));
		moduleList.push_back(std::shared_ptr<IModule>(new NameTags()));
		moduleList.push_back(std::shared_ptr<IModule>(new Criticals()));
		moduleList.push_back(std::shared_ptr<IModule>(new Bhop()));
		moduleList.push_back(std::shared_ptr<IModule>(new Tower()));
		moduleList.push_back(std::shared_ptr<IModule>(new AntiVoid()));
		moduleList.push_back(std::shared_ptr<IModule>(new MidClick()));
		moduleList.push_back(std::shared_ptr<IModule>(new Teleport()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoFriends()));
		moduleList.push_back(std::shared_ptr<IModule>(new Spammer()));
		moduleList.push_back(std::shared_ptr<IModule>(new ChestAura()));
		moduleList.push_back(std::shared_ptr<IModule>(new AntiBot()));
		moduleList.push_back(std::shared_ptr<IModule>(new InventoryCleaner()));
		moduleList.push_back(std::shared_ptr<IModule>(new Derp()));
		moduleList.push_back(std::shared_ptr<IModule>(new Crasher()));
		moduleList.push_back(std::shared_ptr<IModule>(new Placer()));
		moduleList.push_back(std::shared_ptr<IModule>(new CrystalAura()));
		moduleList.push_back(std::shared_ptr<IModule>(new CrystalAuraOW()));
		moduleList.push_back(std::shared_ptr<IModule>(new Disabler()));
		moduleList.push_back(std::shared_ptr<IModule>(new Timer()));
		moduleList.push_back(std::shared_ptr<IModule>(new NightMode()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoSwing()));
		//moduleList.push_back(std::shared_ptr<IModule>(new CubeGlide()));
		moduleList.push_back(std::shared_ptr<IModule>(new AirStuck()));
		moduleList.push_back(std::shared_ptr<IModule>(new Zoom()));
		moduleList.push_back(std::shared_ptr<IModule>(new Teams()));
		moduleList.push_back(std::shared_ptr<IModule>(new Nbt()));
		//moduleList.push_back(std::shared_ptr<IModule>(new Godmode()));
		moduleList.push_back(std::shared_ptr<IModule>(new Freelook()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoSneak()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoHurtcam()));
		moduleList.push_back(std::shared_ptr<IModule>(new AntiImmobile()));
		moduleList.push_back(std::shared_ptr<IModule>(new Waypoints()));
		moduleList.push_back(std::shared_ptr<IModule>(new TimeChanger()));
		moduleList.push_back(std::shared_ptr<IModule>(new	Spider()));
		moduleList.push_back(std::shared_ptr<IModule>(new	Compass()));
		moduleList.push_back(std::shared_ptr<IModule>(new  Radar()));
		moduleList.push_back(std::shared_ptr<IModule>(new VanillaPlus()));
		moduleList.push_back(std::shared_ptr<IModule>(new ViewModel()));
		moduleList.push_back(std::shared_ptr<IModule>(new Twerk()));
		moduleList.push_back(std::shared_ptr<IModule>(new FollowPathModule()));
		moduleList.push_back(std::shared_ptr<IModule>(new FontChanger()));
		moduleList.push_back(std::shared_ptr<IModule>(new Watermark()));
		moduleList.push_back(std::shared_ptr<IModule>(new Theme()));
		moduleList.push_back(std::shared_ptr<IModule>(new Chat()));
		moduleList.push_back(std::shared_ptr<IModule>(new Welcome()));
		moduleList.push_back(std::shared_ptr<IModule>(new Switcher()));
		moduleList.push_back(std::shared_ptr<IModule>(new FlyBypass()));
		moduleList.push_back(std::shared_ptr<IModule>(new autoWalk()));
		moduleList.push_back(std::shared_ptr<IModule>(new NoEntityRender()));
		moduleList.push_back(std::shared_ptr<IModule>(new BoatSpeed()));
		moduleList.push_back(std::shared_ptr<IModule>(new BoatFly()));
		moduleList.push_back(std::shared_ptr<IModule>(new Verification()));
		moduleList.push_back(std::shared_ptr<IModule>(new ArrayList()));
		moduleList.push_back(std::shared_ptr<IModule>(new TargetHUD()));
		moduleList.push_back(std::shared_ptr<IModule>(new InventoryView()));
		moduleList.push_back(std::shared_ptr<IModule>(new Watermark2()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoLog()));
		moduleList.push_back(std::shared_ptr<IModule>(new SilentSwitch()));
		moduleList.push_back(std::shared_ptr<IModule>(new CrystalAuraNukkit()));
		moduleList.push_back(std::shared_ptr<IModule>(new PlayerList()));
		moduleList.push_back(std::shared_ptr<IModule>(new CpsAura()));
		moduleList.push_back(std::shared_ptr<IModule>(new AutoHotbar()));
		moduleList.push_back(std::shared_ptr<IModule>(new ElytraFly()));
		moduleList.push_back(std::shared_ptr<IModule>(new AntiSurround()));
		moduleList.push_back(std::shared_ptr<IModule>(new The2b2eSpeed()));
#ifdef _DEBUG
#endif

		// Sort modules alphabetically
		std::sort(moduleList.begin(), moduleList.end(), [](auto lhs, auto rhs) {
			auto current = lhs;
			auto other = rhs;
			return std::string{*current->getModuleName()} < std::string{*other->getModuleName()};
		});

		initialized = true;
	}
	
#ifdef _BETA
	
#endif  // DEBUG

	getModule<HudModule>()->setEnabled(true);
	getModule<ClickGuiMod>()->setEnabled(false);
	getModule<AntiBot>()->setEnabled(true);
	this->getModule<Verification>()->setEnabled(true);
}

void ModuleManager::disable() {
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		if (mod->isEnabled())
			mod->setEnabled(false);
	}
}

void ModuleManager::onLoadConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		mod->onLoadConfig(conf);
	}

	getModule<HudModule>()->setEnabled(true);
	getModule<ClickGuiMod>()->setEnabled(false);
	getModule<AntiBot>()->setEnabled(true);

#ifdef _BETA
	
#endif  // DEBUG
	this->getModule<Verification>()->setEnabled(true);
}

void ModuleManager::onSaveConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		mod->onSaveConfig(conf);
	}
}

void ModuleManager::onPlayerTick(C_Player* plr) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPlayerTick(plr);
	}
}


void ModuleManager::onWorldTick(C_GameMode* gameMode) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onWorldTick(gameMode);
	}
}

void ModuleManager::onTick(C_GameMode* gameMode) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onTick(gameMode);
	}
}

void ModuleManager::onAttack(C_Entity* attackEnt) {
	if (!isInitialized())
		return;

	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onAttack(attackEnt);
	}
}

void ModuleManager::onKeyUpdate(int key, bool isDown) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		mod->onKeyUpdate(key, isDown);
	}
}

void ModuleManager::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	auto mutex = lockModuleList();

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPreRender(renderCtx);
	}
}

void ModuleManager::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	auto mutex = lockModuleList();

	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPostRender(renderCtx);
	}
}

void ModuleManager::onSendPacket(C_Packet* packet) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onSendPacket(packet);
	}
}

std::vector<std::shared_ptr<IModule>>* ModuleManager::getModuleList() {
	return &moduleList;
}

int ModuleManager::getModuleCount() {
	return (int)moduleList.size();
}

int ModuleManager::getEnabledModuleCount() {
	int i = 0;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled()) i++;
	}
	return i;
}
void ModuleManager::onMove(C_MoveInputHandler* hand) {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onMove(hand);
	}
}
void ModuleManager::onLevelRender() {
	if (!isInitialized())
		return;
	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onLevelRender();
	}
}

ModuleManager* moduleMgr = new ModuleManager(&g_Data);
