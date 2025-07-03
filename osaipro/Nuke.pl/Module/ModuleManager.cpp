#include "ModuleManager.h"
using json = nlohmann::json;
ModuleManager::ModuleManager(GameData* gameData) {
	gameData = gameData;
}
ModuleManager::~ModuleManager() {
	initialized = false;
	moduleList.clear();
}
void AwA() {
	bool msg = false;
	std::string name = "!inMenu";
	if (!msg) {
		if (!Utils::Verify()) Game.terminate();
		if (Game.getLocalPlayer() != nullptr) {
			name = std::string(Game.getLocalPlayer()->getNameTag()->getText());
			Game.getClientInstance()->getGuiData()->displayClientMessageF(
				u8"%sログインしました！%sへようこそ%s!",
				Utils::getCommandClientName(),
				Utils::getClientName(),
				name
			);
		};
		std::string nicestring = Utils::getClientName() +
			std::string(" : Welcome to " + Utils::getClientName() +
				" v" + Utils::getVersion() + " : " + name);
		Notifications::addNotifBox(nicestring, 10.f);
		std::string uniqueID = Utils::generateEncryptedUsername(name);
		Notifications::addNotifBox("Your unique ID is: " + uniqueID, 15.f);
	}
};

std::string catToName(Category cat) {
	std::string categoryName;
	{
		switch (cat) {
		case Category::COMBAT:
			categoryName = "Combat";
			break;
		case Category::VISUAL:
			categoryName = "Render";
			break;
		case Category::MOVEMENT:
			categoryName = "Movement";
			break;
		case Category::PLAYER:
			categoryName = "Player";
			break;
		case Category::WORLD:
			categoryName = "World";
			break;
		case Category::MISC:
			categoryName = "Misc";
			break;
		case Category::CUSTOM:
			categoryName = "Scripts";
			break;
		case Category::CLIENT:
			categoryName = "HUD";
			break;
		}
	}
	return categoryName;
}
#include "../Module/Modules/CrysKa.h"
#include "../Module/Modules/Console.h"
#include "../Module/Modules/HudEditor.h"
#include "../Module/Modules/DragCoordinates.h"
#include "../Module/Modules/DragWelcome.h"
#include "../Module/Modules/DragWelcome.h"

void ModuleManager::initModules() {
	{
		moduleList.emplace_back(new AutoAnvilCrystal());
		moduleList.emplace_back(new CombatInfo());
		moduleList.emplace_back(new CameraNoClip());
		moduleList.emplace_back(new AnchorAura());
		moduleList.emplace_back(new IRC());
		moduleList.emplace_back(new ConsoleMod());
		//bmoduleList.emplace_back(new ArrayListNuke());
//		moduleList.emplace_back(new ToggleSounds());
		moduleList.emplace_back(new Acceleration());
		moduleList.emplace_back(new HudEditor());
		moduleList.emplace_back(new AutoEnchant());
		moduleList.emplace_back(new DragCoordinates());
		moduleList.emplace_back(new DragWelcome());
		moduleList.emplace_back(new FullAutoPiston());
		moduleList.emplace_back(new CrystalKa());
		moduleList.emplace_back(new PointBreak());
		moduleList.emplace_back(new ChatBot());
		moduleList.emplace_back(new TNTAura());
		moduleList.emplace_back(new PacketGapple()); 
		moduleList.emplace_back(new AutoDupe());
		moduleList.emplace_back(new Emote());
		moduleList.emplace_back(new ElytraFly());
		moduleList.emplace_back(new Greeter());
		moduleList.emplace_back(new AutoReply());
		moduleList.emplace_back(new AntiCrystal());
		moduleList.emplace_back(new Desync());
		moduleList.emplace_back(new Silent());
		moduleList.emplace_back(new PopCounter());
		moduleList.emplace_back(new NoClip());
		moduleList.emplace_back(new NoEntityTrace());
		moduleList.emplace_back(new AutoLog());
		moduleList.emplace_back(new BreakIndicator());
		moduleList.emplace_back(new AntiAnvil());
		moduleList.emplace_back(new AutoHotbar());
		moduleList.emplace_back(new PlayerListx());
		moduleList.emplace_back(new CustomHotbar());
		moduleList.emplace_back(new BedAuraRewrite());
		moduleList.emplace_back(new Clip());
		moduleList.emplace_back(new FastUse());
		moduleList.emplace_back(new SelfTrap());
		moduleList.emplace_back(new selectionHighlight());
		moduleList.emplace_back(new Notifications());
		moduleList.emplace_back(new ChestStealer());
		moduleList.emplace_back(new ChatSuffix());
		moduleList.emplace_back(new NukeSpeed());
		moduleList.emplace_back(new ClientTheme());
		moduleList.emplace_back(new ClickGuiMod());
		moduleList.emplace_back(new FastEat());
		moduleList.emplace_back(new FullBright());
		moduleList.emplace_back(new NoSlowDown());
		moduleList.emplace_back(new InstaBreak());
		moduleList.emplace_back(new AutoSprint());
		moduleList.emplace_back(new Jetpack());
		moduleList.emplace_back(new ViewModel());
		moduleList.emplace_back(new Arraylist());
		moduleList.emplace_back(new NoHurtcam());
		moduleList.emplace_back(new AutoSneak());
		moduleList.emplace_back(new AutoArmor());
		moduleList.emplace_back(new HudModule());
		moduleList.emplace_back(new FastStop());
		moduleList.emplace_back(new MidClick());
		moduleList.emplace_back(new NameTags());
		moduleList.emplace_back(new Velocity());
		moduleList.emplace_back(new Killaura());
		moduleList.emplace_back(new SlowSwing());
		moduleList.emplace_back(new AntiBot());
		moduleList.emplace_back(new Spammer());
		moduleList.emplace_back(new Freecam());
		moduleList.emplace_back(new Spider());
		moduleList.emplace_back(new Timer());
		moduleList.emplace_back(new Timer1());
		moduleList.emplace_back(new HoleEsp());
		moduleList.emplace_back(new HoleTP());
		moduleList.emplace_back(new Timer2());
		moduleList.emplace_back(new Jesus());
		moduleList.emplace_back(new BlockScan());
		moduleList.emplace_back(new BreadCrumbs());
		moduleList.emplace_back(new MoreFov());
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
		moduleList.emplace_back(new HoleFill());
		moduleList.emplace_back(new AutoAntiCrystal());
		moduleList.emplace_back(new HatRender());
		moduleList.emplace_back(new AutoAnvil());
		moduleList.emplace_back(new Center());
		moduleList.emplace_back(new FlyBypass());
		moduleList.emplace_back(new hub());
		moduleList.emplace_back(new Kit());
		moduleList.emplace_back(new SelfAnvil());
		moduleList.emplace_back(new cascaffold());
		moduleList.emplace_back(new PacketLogger());
		moduleList.emplace_back(new AutoCrystal());
		moduleList.emplace_back(new AutoTotem());
		moduleList.emplace_back(new NoRender());
		moduleList.emplace_back(new Surround());
		moduleList.emplace_back(new HitBoxes());
		// Sort modules alphabetically
		std::sort(moduleList.begin(), moduleList.end(), [](auto lhs, auto rhs) {
			auto current = lhs;
			auto other = rhs;
			return std::string{*current->getModuleName()} < std::string{*other->getModuleName()};
		});
		initialized = true;
		for (auto LOL : moduleList) {
			Utils::logC("Added Modules " + Utils::changeConsoleColor(2) + std::string(LOL->getModuleName()) + Utils::changeConsoleColor(0) + " to Category " + Utils::changeConsoleColor(3) + std::string(catToName(LOL->getCategory())), "INFO");
		};
	}
	AwA();
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
bool AddedModule = false;
bool sendKey = true;
std::string AccountName = "";
void ModuleManager::onTick(GameMode* gameMode) {
	if (!isInitialized())
		return;
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

bool ModuleManager::AutoDesync(bool timer) {
	return false;
}
ModuleManager* moduleMgr = new ModuleManager(&Game);