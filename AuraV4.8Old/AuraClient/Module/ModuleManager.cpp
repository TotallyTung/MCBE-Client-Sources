#include "ModuleManager.h"
#include "../../Utils/Logger.h"
#include "../../Utils/Json.hpp"

using json = nlohmann::json;

ModuleManager::ModuleManager(GameData* gameData) {
	this->gameData = gameData;
}

ModuleManager::~ModuleManager() {
	initialized = false;
	auto lock = this->lockModuleListExclusive();
	this->moduleList.clear();
}

void ModuleManager::initModules() {
	{
		auto lock = this->lockModuleListExclusive();

		// Menu
		moduleList.emplace_back(new ConfigManagerMod());
		moduleList.emplace_back(new ClickGUIMod());
		moduleList.emplace_back(new DebugMenu());

		// Combat
		moduleList.emplace_back(new AutoCrystal2());
		//moduleList.emplace_back(new CrystalAuraOW());
		moduleList.emplace_back(new CrystalAuraGK());
		moduleList.emplace_back(new AutoObsidian());
		moduleList.emplace_back(new AntiCrystal());
		//moduleList.emplace_back(new AutoCrystal());
		moduleList.emplace_back(new AutoClicker());
		//moduleList.emplace_back(new CrystalAura());
		moduleList.emplace_back(new TriggerBot());
		moduleList.emplace_back(new TargetDown());
		//moduleList.emplace_back(new DamageBug());
		moduleList.emplace_back(new Criticals());
		moduleList.emplace_back(new BowAimbot());
		moduleList.emplace_back(new Switcher2());
		moduleList.emplace_back(new Surround());
		moduleList.emplace_back(new AutoTrap());
		moduleList.emplace_back(new Killaura());
		//moduleList.emplace_back(new KillauraUwU());
		//moduleList.emplace_back(new KillauraRF());
		moduleList.emplace_back(new FastCrossbow());
		moduleList.emplace_back(new InsideAura());
		moduleList.emplace_back(new RainMode());
		moduleList.emplace_back(new RiptideKillaura());
		//moduleList.emplace_back(new KillauraPlus());
		moduleList.emplace_back(new Switcher());
		moduleList.emplace_back(new HitBoxes());
		moduleList.emplace_back(new BowSpam());
		moduleList.emplace_back(new AutoPot());
		moduleList.emplace_back(new Aimbot());
		moduleList.emplace_back(new TPAura());
		moduleList.emplace_back(new Reach());
		moduleList.emplace_back(new Teams());
		moduleList.emplace_back(new FightBot());

		// Visual
		moduleList.emplace_back(new InventoryView());
		moduleList.emplace_back(new BlockOutline());
		moduleList.emplace_back(new PlayerListx());
		moduleList.emplace_back(new BreadCrumbs());
		moduleList.emplace_back(new Animations());
		moduleList.emplace_back(new ArrayList());
		moduleList.emplace_back(new CustomSky());
		moduleList.emplace_back(new pvpResources());
		moduleList.emplace_back(new Interface());
		moduleList.emplace_back(new TargetHUD());
		moduleList.emplace_back(new MoreFov());
		moduleList.emplace_back(new Watermark());
		moduleList.emplace_back(new HoleEsp());
		moduleList.emplace_back(new Ambience());
		moduleList.emplace_back(new ViewModel());
		moduleList.emplace_back(new BlockESP());
		moduleList.emplace_back(new ChestESP());
		moduleList.emplace_back(new CombatInfo());
		moduleList.emplace_back(new Freelook());
		moduleList.emplace_back(new NameTags());
		moduleList.emplace_back(new NoRender());
		moduleList.emplace_back(new Welcome());
		moduleList.emplace_back(new Search());
		moduleList.emplace_back(new Camera());
		moduleList.emplace_back(new SessionInfo());
		moduleList.emplace_back(new Radar());
		moduleList.emplace_back(new Zoom());
		moduleList.emplace_back(new Xray());
		moduleList.emplace_back(new ESP());

		// Movement
		moduleList.emplace_back(new TargetStrafe());
		moduleList.emplace_back(new ElytraFly());
		moduleList.emplace_back(new AntiVoid());
		moduleList.emplace_back(new FastStop());
		moduleList.emplace_back(new LongJump());
		moduleList.emplace_back(new Velocity());
		moduleList.emplace_back(new Jetpack());
		moduleList.emplace_back(new InvMove());
		moduleList.emplace_back(new NoClip());
		moduleList.emplace_back(new NoClipGlide());
		moduleList.emplace_back(new autoWalk());
		moduleList.emplace_back(new Flight());
		moduleList.emplace_back(new NoSlow());
		moduleList.emplace_back(new Spider());
		moduleList.emplace_back(new Sprint());
		moduleList.emplace_back(new Jesus());
		moduleList.emplace_back(new Sneak());
		moduleList.emplace_back(new Speed());
		moduleList.emplace_back(new Step());
		moduleList.emplace_back(new Bhop());
		moduleList.emplace_back(new Glide());

		// Player
		moduleList.emplace_back(new ChestStealer());
		moduleList.emplace_back(new HealthCheck());
		moduleList.emplace_back(new NoJumpDelay());
		moduleList.emplace_back(new BlockReach());
		moduleList.emplace_back(new AntiAnvil());
		moduleList.emplace_back(new AutoGapple());
		moduleList.emplace_back(new AutoHotbar());
		//moduleList.emplace_back(new InvManager());
		moduleList.emplace_back(new AutoTotem());
		moduleList.emplace_back(new PacketMine());
		moduleList.emplace_back(new AutoLog());
		moduleList.emplace_back(new FastPlace());
		moduleList.emplace_back(new AutoArmor());
		moduleList.emplace_back(new Emote());
		moduleList.emplace_back(new Scaffold());
		moduleList.emplace_back(new Freecam());
		moduleList.emplace_back(new NoSwing());
		moduleList.emplace_back(new FastEat());
		moduleList.emplace_back(new Blink());
		moduleList.emplace_back(new Timer());
		moduleList.emplace_back(new XP());

		// Exploit
		moduleList.emplace_back(new AntiImmobile());
		moduleList.emplace_back(new Auto32k());
		//moduleList.emplace_back(new ChatBypass());
		moduleList.emplace_back(new NoLagBack());
		moduleList.emplace_back(new SpeedMine2());
		moduleList.emplace_back(new ChatAttributes());
		moduleList.emplace_back(new ClientSpoof());
		moduleList.emplace_back(new KillInsult());
		moduleList.emplace_back(new SpeedMine());
		moduleList.emplace_back(new PingSpoof());
		moduleList.emplace_back(new EntityFly());
		moduleList.emplace_back(new NoPacket());
		moduleList.emplace_back(new FastUse());
		moduleList.emplace_back(new Crasher());
		moduleList.emplace_back(new FreeTP());
		moduleList.emplace_back(new NoFall());
		moduleList.emplace_back(new Phase());

		// Other
		moduleList.emplace_back(new AutoConfig());
		moduleList.emplace_back(new AutoEz());
		moduleList.emplace_back(new AutoGG());
		moduleList.emplace_back(new ToggleSounds());
		moduleList.emplace_back(new HitEffects());
		moduleList.emplace_back(new ChatSuffix());
		moduleList.emplace_back(new PopCounter());
		//moduleList.emplace_back(new Theme());
		moduleList.emplace_back(new Announcer());
		//moduleList.emplace_back(new Killsults());
		moduleList.emplace_back(new Spammer()); //crash after sending all messages wtf
		moduleList.emplace_back(new ClickTP());
		moduleList.emplace_back(new ItemTP());
		moduleList.emplace_back(new Verify());
		moduleList.emplace_back(new Nuker());
		moduleList.emplace_back(new Derp());
		moduleList.emplace_back(new MCF());
		moduleList.emplace_back(new PacketLogger());

#ifdef _DEBUG
		moduleList.emplace_back(new PacketLogger());
		moduleList.emplace_back(new TestModule());
		//moduleList.emplace_back(new FallSave());
#endif

		std::sort(moduleList.begin(), moduleList.end(), [](auto lhs, auto rhs) {
			auto current = lhs;
			auto other = rhs;
			return std::string{*current->getModuleName()} < std::string{*other->getModuleName()};
		});
		initialized = true;
	}
	
	//getModule<Notifications>()->setEnabled(true);
	getModule<ClickGUIMod>()->setEnabled(false);
	getModule<Killaura>()->setEnabled(false);
	getModule<Interface>()->setEnabled(true);
	getModule<ArrayList>()->setEnabled(true);
	getModule<Watermark>()->setEnabled(true);
	getModule<Verify>()->setEnabled(true);
	//getModule<AntiBot>()->setEnabled(true);
	//getModule<AntiBot>()->setEnabled(true);
	//getModule<Sprint>()->setEnabled(true);
}

void ModuleManager::disable() {
	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		if (mod->isEnabled())
			mod->setEnabled(false);
	}
}

void ModuleManager::onLoadConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized()) return;

	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		mod->onLoadConfig(conf);
	}

	if (!getModule<Step>()->isEnabled() && g_Data.getLocalPlayer() != nullptr) g_Data.getLocalPlayer()->stepHeight = 0.5625f;
	getModule<ClickGUIMod>()->setEnabled(false);
}

void ModuleManager::onSaveConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized()) return;

	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		mod->onSaveConfig(conf);
	}
}

void ModuleManager::onLoadSettings(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized()) return;

	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		mod->onLoadSettings(conf);
	}
	getModule<ClickGUIMod>()->setEnabled(false);
}

void ModuleManager::onSaveSettings(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized()) return;

	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		mod->onSaveSettings(conf);
	}
}

bool ModuleManager::AutoDesync(bool timer) {
	if (g_Data.getLocalPlayer() != nullptr) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		C_ItemStack* Items = inv->getItemStack(supplies->selectedHotbarSlot);
		if (Items->item == nullptr) return false;
		if ((Items->getItem()->isFood()) && g_Data.isRightClickDown()) {
			if (!timer)
				return true;
			else if (timer)
			{
				g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f); return true;
			}
		}
	}
	else return true;
	return false;
}

void ModuleManager::onPlayerTick(C_Player* plr) {
	if (!isInitialized()) return;

	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPlayerTick(plr);
	}
}

void ModuleManager::onWorldTick(C_GameMode* gameMode) {
	if (!isInitialized()) return;

	auto lock = lockModuleList();
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onWorldTick(gameMode);
	}
}

void ModuleManager::onTick(C_GameMode* gameMode) {
	if (!isInitialized()) return;

	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onTick(gameMode);
	}
}

void ModuleManager::onAttack(C_Entity* attackEnt) {
	if (!isInitialized()) return;

	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onAttack(attackEnt);
	}
}

void ModuleManager::onKeyUpdate(int key, bool isDown) {
	if (!isInitialized()) return;

	auto lock = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		mod->onKeyUpdate(key, isDown);
	}
}

void ModuleManager::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized()) return;

	auto mutex = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPreRender(renderCtx);
	}
}

void ModuleManager::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized()) return;

	auto mutex = this->lockModuleList();
	for (auto& mod : this->moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPostRender(renderCtx);
	}
}

void ModuleManager::onSendPacket(C_Packet* packet) {
	if (!isInitialized()) return;

	auto lock = lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onSendPacket(packet);
	}
}

std::vector<std::shared_ptr<IModule>>* ModuleManager::getModuleList() {
	return &this->moduleList;
}

int ModuleManager::getModuleCount() {
	return (int)moduleList.size();
}

int ModuleManager::getEnabledModuleCount() {
	int i = 0;
	auto lock = this->lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled()) i++;
	}
	return i;
}

void ModuleManager::onMove(C_MoveInputHandler* hand) {
	if (!isInitialized()) return;

	auto lock = this->lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onMove(hand);
	}
}
void ModuleManager::ClickGuiChanger() {
	if (!isInitialized()) return;

	for (auto& it : moduleList) {
		it->ClickGuiChanger();
	}
}
void ModuleManager::onLevelRender() {
	if (!isInitialized()) return;
	auto lock = this->lockModuleList();
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled()) it->onLevelRender();
	}
}

ModuleManager* moduleMgr = new ModuleManager(&g_Data);