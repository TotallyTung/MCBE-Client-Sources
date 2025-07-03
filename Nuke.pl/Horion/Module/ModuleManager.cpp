#include "ModuleManager.h"
#include "Modules/FastUse.h"

using json = nlohmann::json;

ModuleManager::ModuleManager(GameData* gameData) {
	gameData = gameData;
}

ModuleManager::~ModuleManager() {
	initialized = false;
	moduleList.clear();
}

void ModuleManager::initModules() {
	Notifications::addNotifBox("Successfully injected " + Utils::getClientName() + "!", 5.f);
	logF("Initializing modules.");
	{
		moduleList.emplace_back(new AntiCrystal());
		moduleList.emplace_back(new PopCounter());
		moduleList.emplace_back(new NoClip());
		moduleList.emplace_back(new AutoEZ());
		moduleList.emplace_back(new AutoLog());
		moduleList.emplace_back(new FastAutoLog());
		moduleList.emplace_back(new AntiAnvil());
		moduleList.emplace_back(new AutoHotbar());
		moduleList.emplace_back(new PlayerListx());
		moduleList.emplace_back(new PhaseForAntiAnvil());
		moduleList.emplace_back(new GravityAura());
		moduleList.emplace_back(new AutoPhase());
		moduleList.emplace_back(new FastUse());
		moduleList.emplace_back(new SelfTrap());
		moduleList.emplace_back(new InventoryCleaner());
		// moduleList.emplace_back(new InfiniteAura());
		moduleList.emplace_back(new InventoryMove());
		moduleList.emplace_back(new BlockOutline());
		moduleList.emplace_back(new Notifications());
		moduleList.emplace_back(new AntiImmobile());
		moduleList.emplace_back(new ChestStealer());
		moduleList.emplace_back(new ChatSuffix());
		moduleList.emplace_back(new NukeSpeed());
		moduleList.emplace_back(new EditionFaker());
		moduleList.emplace_back(new ClientTheme());
		// moduleList.emplace_back(new CubeGlide());
		moduleList.emplace_back(new CrystalAura());
		moduleList.emplace_back(new ClickGuiMod());
		// moduleList.emplace_back(new AirStuck());
		moduleList.emplace_back(new BlockReach());
		moduleList.emplace_back(new FastEat());
		moduleList.emplace_back(new FullBright());
		moduleList.emplace_back(new NoSlowDown());
		moduleList.emplace_back(new InstaBreak());
		moduleList.emplace_back(new AutoSprint());
		moduleList.emplace_back(new StorageESP());
		moduleList.emplace_back(new Jetpack());
		moduleList.emplace_back(new ViewModel());
		moduleList.emplace_back(new Arraylist());
		moduleList.emplace_back(new NoHurtcam());
		moduleList.emplace_back(new AutoSneak());
		// moduleList.emplace_back(new Godmode());
		moduleList.emplace_back(new ChestAura());
		moduleList.emplace_back(new NoFriends());
		moduleList.emplace_back(new AutoArmor());
		moduleList.emplace_back(new BowAimbot());
		moduleList.emplace_back(new Criticals());
		moduleList.emplace_back(new HudModule());
		moduleList.emplace_back(new FastStop());
		moduleList.emplace_back(new Freelook());
		moduleList.emplace_back(new Teleport());
		moduleList.emplace_back(new MidClick());
		moduleList.emplace_back(new NameTags());
		moduleList.emplace_back(new HighJump());
		moduleList.emplace_back(new NoPacket());
		moduleList.emplace_back(new Scaffold());
		moduleList.emplace_back(new Velocity());
		moduleList.emplace_back(new Killaura());
		moduleList.emplace_back(new NoSwing());
		moduleList.emplace_back(new SlowSwing());
		moduleList.emplace_back(new AntiBot());
		moduleList.emplace_back(new Spammer());
		moduleList.emplace_back(new Freecam());
		moduleList.emplace_back(new AirJump());
		moduleList.emplace_back(new Spider());
		moduleList.emplace_back(new Hitbox());
		moduleList.emplace_back(new NoFall());
		moduleList.emplace_back(new Tracer());
		moduleList.emplace_back(new Timer());
		moduleList.emplace_back(new Timer1());
		moduleList.emplace_back(new HoleEsp());
		moduleList.emplace_back(new HoleTP());
		moduleList.emplace_back(new Timer2());
		moduleList.emplace_back(new Tower());
		moduleList.emplace_back(new Jesus());
		moduleList.emplace_back(new NoWeb());
		moduleList.emplace_back(new Reach());
		moduleList.emplace_back(new Blink());
		moduleList.emplace_back(new Phase());
		moduleList.emplace_back(new Glide());
		moduleList.emplace_back(new Zoom());
		moduleList.emplace_back(new Xray());
		moduleList.emplace_back(new Step());
		moduleList.emplace_back(new Nbt());
		moduleList.emplace_back(new Fly());
		moduleList.emplace_back(new AutoButton());
		moduleList.emplace_back(new ESP());
		moduleList.emplace_back(new CustomFont());
		moduleList.emplace_back(new Colors());
		moduleList.emplace_back(new PacketMine());
		moduleList.emplace_back(new AutoTrap());
		moduleList.emplace_back(new AutoTool());
		moduleList.emplace_back(new HoleFill());
		moduleList.emplace_back(new AutoAntiCrystal());
		moduleList.emplace_back(new HatRender());
		moduleList.emplace_back(new AutoAnvil());
		moduleList.emplace_back(new Center());
		moduleList.emplace_back(new FlyBypass());
		moduleList.emplace_back(new AutoDupe());
		moduleList.emplace_back(new SelfAnvil());
		moduleList.emplace_back(new cascaffold());
		moduleList.emplace_back(new PacketLogger());
		moduleList.emplace_back(new AutoCrystal());
		moduleList.emplace_back(new Offhand());
		moduleList.emplace_back(new NoRender());
		moduleList.emplace_back(new Surround());

		moduleList.emplace_back(new CubeSpeed());
		moduleList.emplace_back(new Bhop());
		moduleList.emplace_back(new Speed());
		// Sort modules alphabetically
		std::sort(moduleList.begin(), moduleList.end(), [](auto lhs, auto rhs) {
			auto current = lhs;
			auto other = rhs;
			return std::string{*current->getModuleName()} < std::string{*other->getModuleName()};
		});

		initialized = true;
	}
	
	getModule<ClickGuiMod>()->setEnabled(false);
	getModule<HudModule>()->setEnabled(true);
	getModule<AntiBot>()->setEnabled(true);
	
}

void ModuleManager::disable() {
	for (auto& mod : moduleList) {
		if (mod->isEnabled())
			mod->setEnabled(false);
	}
	
}

void ModuleManager::onLoadConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized())
		return;
	for (auto& mod : moduleList) {
		mod->onLoadConfig(conf);
	}

	getModule<ClickGuiMod>()->setEnabled(false);
	
}

void ModuleManager::onSaveConfig(void* confVoid) {
	auto conf = reinterpret_cast<json*>(confVoid);
	if (!isInitialized())
		return;
	for (auto& mod : moduleList) {
		mod->onSaveConfig(conf);
	}
	
}

void ModuleManager::onWorldTick(GameMode* gameMode) {
	if (!isInitialized())
		return;
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onWorldTick(gameMode);
	}
	
}
bool a = false;
void ModuleManager::onTick(GameMode* gameMode) {
	if (!isInitialized())
		return;
	if (!a) {
		a = Utils::Verify();
	};
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onTick(gameMode);
	}
	
}

void ModuleManager::onAttack(Entity* attackEnt) {
	if (!isInitialized())
		return;
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onAttack(attackEnt);
	}
	
}

void ModuleManager::onKeyUpdate(int key, bool isDown) {
	if (!isInitialized())
		return;
	for (auto& mod : moduleList) {
		mod->onKeyUpdate(key, isDown);
	}
	
}

void ModuleManager::onKey(int key, bool isDown, bool& shouldCancel) {
	if (!isInitialized())
		return;
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onKey(key, isDown, shouldCancel);
	}
	
}

void ModuleManager::onPreRender(MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPreRender(renderCtx);
	}
	
}

void ModuleManager::onPostRender(MinecraftUIRenderContext* renderCtx) {
	if (!isInitialized())
		return;
	for (auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled())
			mod->onPostRender(renderCtx);
	}
	
}

void ModuleManager::onSendClientPacket(Packet* packet) {
	if (!isInitialized())
		return;
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onSendClientPacket(packet);
	}
	
}

void ModuleManager::onSendPacket(Packet* packet) {
	if (!isInitialized())
		return;
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onSendPacket(packet);
	}
	
}

void ModuleManager::onBaseTick(Entity* ent) {
	if (!isInitialized())
		return;
	for (auto& mod : moduleList) {
		mod->onBaseTick(ent);
	}
	
}

void ModuleManager::onImGuiRender(ImDrawList* d) {
	if (!isInitialized())
		return;
	for (const auto& mod : moduleList) {
		if (mod->isEnabled() || mod->callWhenDisabled()) {
			mod->onImGuiRender(d);
		}
	}
	
}

void ModuleManager::onJoiningServer() {
	if (!isInitialized())
		return;
	for (const auto& mod : moduleList) {
		mod->onJoiningServer(); 
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
	for (auto& it : moduleList) {
		if (it->isEnabled()) i++;
	}
	
	return i;
}
void ModuleManager::onMove(MoveInputHandler* hand) {
	
	if (!isInitialized())
		return;
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onMove(hand);
	}
}

void ModuleManager::onPlayerTick(Player* player) {
	
	if (!isInitialized())
		return;
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onPlayerTick(player);
	}
}
void ModuleManager::onLevelRender() {
	
	if (!isInitialized())
		return;
	for (auto& it : moduleList) {
		if (it->isEnabled() || it->callWhenDisabled())
			it->onLevelRender();
	}
}

ModuleManager* moduleMgr = new ModuleManager(&Game);
